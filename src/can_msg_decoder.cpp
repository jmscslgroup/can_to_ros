#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <bitset>
#include <math.h>
#include <ctime>

class decode_msgs{

private:
float speed;
float steering_angle;

public:
decode_msgs(): speed(0.0) , steering_angle(0.0){}; // constructor 
float GetSpeed(){ return this->speed; };
float GetSteeringAngle(){ return this->steering_angle; };

int decode_message( unsigned int msg_id, std::string msg); // decoding CAN messages

std::string findTwoscomplement(std::string str); // functon to return two's complement 

};


std::string decode_msgs::findTwoscomplement(std::string str) { 
    int n = str.length(); 
  
    // Traverse the string to get first '1' from 
    // the last of string 
    int i; 
    for (i = n-1 ; i >= 0 ; i--) 
        if (str[i] == '1') 
            break; 
  
    // If there exists no '1' concatenate 1 at the 
    // starting of string 
    if (i == -1) 
        return '1' + str; 
  
    // Continue traversal after the position of 
    // first '1' 
    for (int k = i-1 ; k >= 0; k--) 
    { 
        //Just flip the values 
        if (str[k] == '1') 
            str[k] = '0'; 
        else
            str[k] = '1'; 
    } 
    // return the modified string 
    return str;
} 

int decode_msgs::decode_message( unsigned int msg_id, std::string msg){
  int raw_angle;
  unsigned long long int n;
  std::string binary;
  unsigned long long  rawVal_Dec;
  float speedDec_kmph;
  float speed_mps;
  std::string byte1;
  std::string byte2;
  std::string byte6;
  std::string byte7;
  std::string temp1;
  std::string rawVal;
  float angle;
  if (msg_id == 180){
  
        std::stringstream hex_ss(msg); 
        hex_ss >> std::hex >> n;// making the message hex 

        binary = std::bitset<64>(n).to_string(); // conver hex to binary 

         byte6 = binary.substr(40,8);
         byte7 = binary.substr(48,8);

        rawVal= byte6+byte7;
       
         rawVal_Dec=std::stoull(rawVal, 0, 2);

        speedDec_kmph= (float)rawVal_Dec * 0.01;
         speed_mps= ceilf((speedDec_kmph* (10.0/36.0)) * 100) / 100;

      this->steering_angle = 0.0;
      this->speed = speed_mps;
      
    return 0;
  }
  if (msg_id == 37){

        std::stringstream hex_ss(msg); // 
        hex_ss >> std::hex >> n;
       /// std::cout << n << std::endl;

       binary = std::bitset<64>(n).to_string(); // conver hex to binary 

         byte1 = binary.substr(0,8);
         byte2 = binary.substr(8,8);

         temp1= byte1.substr(4,4);// getting the last fout bits of the first byte

        if ((temp1+byte2)[0]== '0'){
         raw_angle= std::stoul( temp1+byte2, 0, 2 );
        }
        else {

          raw_angle= std::stoul(findTwoscomplement(temp1+byte2), 0, 2 );
          raw_angle=raw_angle * -1.0;
        }
        angle=float(raw_angle)*1.5;

        this->steering_angle = angle;
        this->speed = 0.0;

        return 0;

  }

}




float getAvgDelta_t(std::string file_name){ // the getAvgDelta_t() calculates the average time difference between the the timestamp
  float results=0.0;
  std::string  inputLine, Bus, Message, Time;
  int  MessageID;
  float sum=0.0;
  float prev_value=0.0;
  float curr_value=0.0;
  int count=0;
  bool firstLine = true;
  int hour;
  int min;
  int sec;
  std::string microseconds;
  std::string time_in_sec;
  float delta;
  std::ifstream inFile;
  inFile.open(file_name);
  if( !inFile.is_open()){// check if the file is opened correctly.
    std::cout << "Cannot open file to read"<< std::endl;
   }
  while (getline(inFile, inputLine)){
    if (firstLine){ // skip the first line
      firstLine = false;
      continue;
      }
    if (inputLine.empty()) continue; // if the line is empty
      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
      std::stringstream ss(inputLine);
      ss >> Time>> Bus>> MessageID;
 
      if (MessageID == 180){
        count++; // counter 
        std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
         microseconds = Time.substr(10,6); // storing the precision value 
  
        struct tm * ptm=(gmtime(&epoch1)); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
        // struct tm is structure holding a calendar date and time broken down into its components.
        hour= ptm->tm_hour;
        min= ptm->tm_min;
        sec= ptm->tm_sec;

        time_in_sec = std::to_string((hour*3600)+(min*60)+sec) + microseconds; // total time in seconds 

        if (count ==1){
            prev_value= stod(time_in_sec);
          }
        if (count > 1){
  
            curr_value=stod(time_in_sec);
            delta=curr_value-prev_value; // taking the difference 
            sum=sum+delta;
            prev_value= stod(time_in_sec);
          }
      
      }
 
   }
      results=sum/(count-1); // calculating the average 
    return results;
  }


/**********************************************************************************************/

int main(int argc, char **argv){
    ros::init(argc, argv, "can_msg_decoder");
    ros::NodeHandle nh("~");
    ros::Publisher chatter_pub = nh.advertise<geometry_msgs::Twist>("/vehicle/vel", 1000);   // pulishing to /vehicle/vel topic
    ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    std::string user_input="";
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Bus,Message,MessageLength;
    int MessageID;
    geometry_msgs::Twist msg;
    bool firstLine=true;
    if (argc != 2){ // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }
  
    float delta_t = getAvgDelta_t(argv[1]); // calling getAvgDelta_t() and assign the returned value to delta_t

    ros::Rate rate(1.0/delta_t); // the publish rate is 1/delta_t
    unsigned long long int n; // variable to hold the hex value 

    inFile.open(argv[1]);

    if( !inFile.is_open()){// check id the file is opened correctly.
      std::cout << "Cannot open file to read"<< std::endl;
      return 1;
    }
    do {  // asking for user input 
    std::cout << "Enter (S) for speed or (A) for steering angle: " << std::endl;
    std::cin >> user_input;
    } while (user_input != "A" && user_input != "S");
    

    while (ros::ok()){

      if (!getline(inFile, inputLine)) break;
      if (firstLine){ // skip the first line
        firstLine=false;
        continue;
      }
     if (inputLine.empty()) continue; // if there is an empty line then skip it

      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
      std::stringstream ss(inputLine);
      ss >> Time>> Bus>> MessageID>> Message>> MessageLength;
 
      obj.decode_message (MessageID, Message);  // speedID 180, steering angleID 37

      if (MessageID == 180 && user_input == "S"){ 
       msg.linear.x = obj.GetSpeed();
      // std::cout << "speed " << obj.GetSpeed() << std::endl;
       }
     if (MessageID == 37 && user_input == "A"){ 
      //outFile << obj.GetSteeringAngle() << "," << Message << std::endl;
      }
    }
     
        chatter_pub.publish(msg);
        ros::spinOnce();
        rate.sleep();
  
      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
