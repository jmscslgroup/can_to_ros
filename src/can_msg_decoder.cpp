#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
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
float lead_dist;

public:
decode_msgs(): speed(0.0) , steering_angle(0.0), lead_dist(0.0) {}; // constructor 
float GetSpeed(){ return this->speed; };
float GetSteeringAngle(){ return this->steering_angle; };
float GetLead_dist() {return this->lead_dist; };

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
  float lead_dist;
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

      //this->steering_angle = 0.0;
      this->speed = speed_mps;
      
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
       // this->speed = 0.0;


  }
    if (msg_id == 869){
  
        std::stringstream hex_ss(msg); 
        hex_ss >> std::hex >> n;// making the message hex 

        binary = std::bitset<56>(n).to_string(); // conver hex to binary 

        std::string byte5 = binary.substr(32,8);
      

        rawVal= byte5;
       
         rawVal_Dec=std::stoull(rawVal, 0, 2);

         lead_dist= (float)rawVal_Dec;

        //this->steering_angle = 0.0;
        //this->speed = 0.0;
        this->lead_dist = rawVal_Dec;
      

  }

return 0;
}


/**********************************************************************************************/

int main(int argc, char **argv){
    ros::init(argc, argv, "can_msg_decoder");
    ros::NodeHandle nh("~");
    ros::NodeHandle nh2("~");
    ros::Publisher speed_pub = nh.advertise<geometry_msgs::Twist>("/vehicle/vel", 1000);   // pulishing to /vehicle/vel topic
    ros::Publisher lead_dist_pub = nh2.advertise<std_msgs::Float32>("/vehicle/distanceEstimator/dist", 1000);  
    ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    std::string user_input="";
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Buffer,Bus,Message,MessageLength;
    int MessageID;
    
    bool firstLine=true;
    if (argc != 2){ // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }
  
    //std::cout << delta_t << "  delta" << std::endl;

    ros::Rate rate(50.0); // the publish rate is 1/delta_t 

    inFile.open(argv[1]);
    if( !inFile.is_open()){// check id the file is opened correctly.
      std::cout << "Cannot open file to read"<< std::endl;
      return 1;
    }
    do {  // asking for user input 
    std::cout << "Enter (S) for speed or (A) for steering angle or (D) for lead distance: " << std::endl;
    std::cin >> user_input;
    } while (user_input != "A" && user_input != "S" && user_input != "D");
    

    while (ros::ok()){

      if (!getline(inFile, inputLine)) break;
      if (firstLine){ // skip the first line
        firstLine=false;
        continue;
      }
     if (inputLine.empty()) continue; // if there is an empty line then skip it

      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
      std::stringstream ss(inputLine);
      ss >> Time>> Buffer>> Bus>> MessageID>> Message>> MessageLength;
 
      obj.decode_message (MessageID, Message);  // speedID 180, steering angleID 37

      if (MessageID == 180 && user_input == "Sdsad"){ 
       std::cout << "speed " << obj.GetSpeed() << std::endl;
       geometry_msgs::Twist msg;
       msg.linear.x = obj.GetSpeed();
       speed_pub.publish(msg);
        ros::spinOnce();
        rate.sleep();
       
       }
     if (MessageID == 37 && user_input == "A"){ 
      //outFile << obj.GetSteeringAngle() << "," << Message << std::endl;
      }


       if (MessageID == 869 && user_input == "D"){ 
       std::cout << "leadDIST " << obj.GetLead_dist() << std::endl;
       std_msgs::Float32 dist;
       dist.data = obj.GetLead_dist();
       lead_dist_pub.publish(dist);
        ros::spinOnce();
       rate.sleep();
      }
 
      
    }
     
      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
