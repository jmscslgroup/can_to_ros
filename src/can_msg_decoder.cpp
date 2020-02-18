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



float getAvgDelta_t(std::string file_name){ // the getAvgDelta_t() calculates the average time difference between the the timestamp
  float results=0.0;
  std::string  inputLine, Bus, Message, Time;
  int  MessageID;
  float sum=0.0;
  float prev_value=0.0;
  float curr_value=0.0;
  int count=0;
  bool firstLine = true;

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
        std::string microseconds = Time.substr(10,6); // storing the precision value 
  
        struct tm * ptm=(gmtime(&epoch1)); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
        // struct tm is structure holding a calendar date and time broken down into its components.
        int hour= ptm->tm_hour;
        int min= ptm->tm_min;
        int sec= ptm->tm_sec;

        std::string time_in_sec = std::to_string((hour*3600)+(min*60)+sec) + microseconds; // total time in seconds 

        if (count ==1){
            prev_value= stod(time_in_sec);
          }
        if (count > 1){
  
            curr_value=stod(time_in_sec);
            float delta=curr_value-prev_value; // taking the difference 
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

    std::string inputLine="";
    std::string Time,Bus,Message,MessageLength;
    int MessageID;
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
 
      if (MessageID == 180){ // MID for speed is 180
        std::stringstream hex_ss(Message); 
        hex_ss >> std::hex >> n; // making the message hex 
        //std::cout << n << std::endl;

        std::string binary = std::bitset<64>(n).to_string();// conver hex to binary 

        // std::string byte1 = binary.substr(0,8);
        // std::string byte2 = binary.substr(8,8);
        // std::string byte3 = binary.substr(16,8);
        // std::string byte4 = binary.substr(24,8);
        // std::string byte5 = binary.substr(32,8);
        std::string byte6 = binary.substr(40,8);
        std::string byte7 = binary.substr(48,8);
        // std::string byte8 = binary.substr(56,8);

        // std::cout << "byte1 " << byte1<< std::endl;
        // std::cout << "byte2 " << byte2<< std::endl;
        // std::cout << "byte3 " << byte3<< std::endl;
        // std::cout << "byte4 " << byte4<< std::endl;
        // std::cout << "byte5 " << byte5<< std::endl;
       // std::cout << "byte6 " << byte6<< std::endl;
        //std::cout << "byte7 " << byte7<< std::endl;
        // std::cout << "byte8 " << byte8<< std::endl;


       std::string rawVal= byte6+byte7;
       
       unsigned long long  rawVal_Dec=std::stoull(rawVal, 0, 2); // converting a string to unsigned long long 

       float speedDec_kmph= (float)rawVal_Dec * 0.01;// multiply the raw value with a factor (0.01 given in the dbc file)
       float speed_mps= ceilf((speedDec_kmph* (10.0/36.0)) * 100) / 100; // converting speed to m/s

       std::cout << "speed: " << speed_mps<< " m/s" <<std::endl;// output to the console
     
        geometry_msgs::Twist msg;
        msg.linear.x = speed_mps;
        chatter_pub.publish(msg);
        ros::spinOnce();
        rate.sleep();
  
      }
    }

      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
