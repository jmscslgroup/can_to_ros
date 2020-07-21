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

typedef struct values_struct
{
   float var1, var2, var3;
} values;

class decode_msgs{
public:
values decode_message( unsigned int msg_id, std::string msg); // decoding CAN messages
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

values decode_msgs::decode_message( unsigned int msg_id, std::string msg){
  values returnedVal;
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
  returnedVal.var1=0.0;
  returnedVal.var2=0.0;
  returnedVal.var3=0.0;
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
      returnedVal.var1 = speed_mps;
      return returnedVal;
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

        returnedVal.var1= angle;
       // this->speed = 0.0;

      return returnedVal;
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
        returnedVal.var1 = rawVal_Dec;
      return returnedVal;

  }
  if (msg_id>= 384 && msg_id<=399 ){
     
        std::stringstream hex_ss(msg);
        hex_ss >> std::hex >> n;

        binary = std::bitset<64>(n).to_string();

        std::string byte2 = binary.substr(8,8);
        std::string byte3 = binary.substr(16,8);
        std::string byte4 = binary.substr(24,8);
        std::string byte5 = binary.substr(32,8);
        std::string byte6 = binary.substr(40,8);
        std::string byte7 = binary.substr(48,8);

       std::string rawVal_lat= byte4+ byte5.substr(0,3);
       std::string rawVal_long=byte2+byte3.substr(0,5);
       std::string raw_rel_speed=byte6+byte7.substr(0,4);

       float rawVal_lat_fl;
       float raw_rel_speed_fl;
             if ((rawVal_lat)[0]== '0'){
         rawVal_lat_fl= std::stoul( rawVal_lat, 0, 2 );
        }
        else {

        rawVal_lat_fl= std::stoul(findTwoscomplement(rawVal_lat), 0, 2 );
          rawVal_lat_fl=rawVal_lat_fl * -1.0;
        }

             if ((raw_rel_speed)[0]== '0'){
         raw_rel_speed_fl= std::stoul( raw_rel_speed, 0, 2 );
        }
        else {

        raw_rel_speed_fl= std::stoul(findTwoscomplement(raw_rel_speed), 0, 2 );
          raw_rel_speed_fl=raw_rel_speed_fl * -1.0;
        }

        float rawVal_long_fl=float(std::stoull(rawVal_long, 0, 2));

        std::cout << rawVal_lat << " " << rawVal_long << " " << raw_rel_speed << std::endl;
        returnedVal.var1=rawVal_lat_fl*0.04;
        returnedVal.var2=rawVal_long_fl*0.04;
        returnedVal.var3=raw_rel_speed_fl*0.025;
        return returnedVal;
  
  }
    if (msg_id >= 400 && msg_id <= 415){
     
        std::stringstream hex_ss(msg);
        hex_ss >> std::hex >> n;

        binary = std::bitset<64>(n).to_string();

        std::string byte2 = binary.substr(8,8);

        std::string raw_accel= byte2.substr(0,7);

        float raw_accel_fl;
             if ((raw_accel)[0]== '0'){
         raw_accel_fl= std::stoul( raw_accel, 0, 2 );
        }
         else {

        raw_accel_fl= std::stoul(findTwoscomplement(raw_accel), 0, 2 );
          raw_accel_fl=raw_accel_fl * -1.0;
        }

        std::cout << raw_accel_fl << std::endl;
        returnedVal.var1=raw_accel_fl*1.0;
        returnedVal;
    }

    if (msg_id == 552){
    int temp_accel;
    std::stringstream hex_ss(msg);
    hex_ss >> std::hex >> n;

    binary = std::bitset<32>(n).to_string();
    //std::cout << binary << std::endl;
    std::string byte1 = binary.substr(0,8);
    std::string byte2 = binary.substr(8,8);
    std::string byte3 = binary.substr(16,8);
    std::string byte4 = binary.substr(24,8);
    // std::cout << byte1 << std::endl;
    // std::cout << byte2 << std::endl;
    // std::cout << byte3 << std::endl;
    // std::cout << byte4 << std::endl;

    temp1= byte1.substr(1,7);
    rawVal= temp1+byte2;
    
    if (rawVal[0]== '0'){
      temp_accel= std::stoul( rawVal, 0, 2 );
    }
    else {

      temp_accel= std::stoul(findTwoscomplement(rawVal), 0, 2 );
      temp_accel=temp_accel * -1.0;
    }
    returnedVal.var1=float(temp_accel)*0.001;
    //std::cout << this->long_accel << "  Accel" << std::endl; 

    return returnedVal;
    }

}

/**********************************************************************************************/

int main(int argc, char **argv){
    ros::init(argc, argv, "strangle_node");
    ros::NodeHandle nh("~");
    ros::Publisher str_angle_pub = nh.advertise<std_msgs::Float32>("/vehicle/steering_angle", 1000);   // pulishing to /vehicle/vel topic
    ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Buffer,Bus,Message,MessageLength;
    int MessageID;
    values data;
    
    bool firstLine=true;
    if (argc != 2){ // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }

    ros::Rate rate(20.0); // the publish rate is 1/delta_t 

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

      if (MessageID == 37){ 
      //  std::cout << "str angle " << obj.GetSteeringAngle() << std::endl;
       data = obj.decode_message (MessageID, Message);
       std_msgs::Float32 str_angle;
       str_angle.data = data.var1;
       str_angle_pub.publish(str_angle);
       ros::spinOnce();
       rate.sleep();
      }

    }
     
      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
