#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/PointCloud2.h"
#include "ainstein_radar_msgs/RadarTarget.h"
#include "ainstein_radar_msgs/RadarTargetArray.h"
#include "ainstein_radar_msgs/RadarInfo.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <bitset>
#include <math.h>
#include <ctime>
#include <thread> 
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "visualization_msgs/Marker.h"

class decode_msgs{

private:
float speed;
float steering_angle;
float lead_dist;
float lat_dist;
float long_dist;
float rel_speed;
float rel_accel;

public:
decode_msgs(): speed(0.0) , steering_angle(0.0), lead_dist(0.0), lat_dist(0.0),long_dist(0.0), rel_speed(0.0), rel_accel(0.0) {}; // constructor 
float GetSpeed(){ return this->speed; };
float GetSteeringAngle(){ return this->steering_angle; };
float GetLead_dist() {return this->lead_dist; };
std::stringstream GetTrackAinfo(){ 
  std::stringstream data;
  data << lat_dist << " " << long_dist << " " <<rel_speed;
  return  data;
}
float GetTrackBinfo(){
  return rel_accel;
}

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

        //std::cout << rawVal_lat << " " << rawVal_long << " " << raw_rel_speed << std::endl;
        this->lat_dist=rawVal_lat_fl*0.04;
        this->long_dist=rawVal_long_fl*0.04;
        this->rel_speed=raw_rel_speed_fl*0.025;
  
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

       // std::cout << raw_accel_fl << std::endl;
        this->rel_accel=raw_accel_fl*1.0;
    }

return 0;
}


/**********************************************************************************************/
void theadF() {
  std::cout << "Im in thread" << std::endl;
}

int main(int argc, char **argv){
  // std::thread t1(theadF);
  //   t1.join();
    ros::init(argc, argv, "node12");
    ros::NodeHandle nh1("~");

    ros::Publisher pose_pub = nh1.advertise<visualization_msgs::Marker>("/targets_pose12", 1000);

    //ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    std::string user_input="";
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Buffer,Bus,Message,MessageLength;
    int MessageID;
    float y,x,rel_spd;
    float az;
    int i=0;
    
    // bool firstLine=true;
    // if (argc != 2){ // check the nunber of the argument
    //     std::cout <<"./a.out .csv" << std::endl;
    //     return 1;
    // }
  
    //std::cout << delta_t << "  delta" << std::endl;

    ros::Rate rate(20.0); // the publish rate is 1/delta_t 

    // inFile.open(argv[1]);
    // if( !inFile.is_open()){// check id the file is opened correctly.
    //   std::cout << "Cannot open file to read"<< std::endl;
    //   return 1;
    // }
    // // do {  // asking for user input 
    // // std::cout << "Enter (S) for speed or (A) for steering angle or (D) for lead distance: " << std::endl;
    // // std::cin >> user_input;
    // // } while (user_input != "A" && user_input != "S" && user_input != "T");
    // ainstein_radar_msgs::RadarTargetArray targetArr;

 while (ros::ok()){
    
     if (!getline(std::cin, inputLine)) break;

     if (inputLine.empty()) continue; // if there is an empty line then skip it

      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
      std::stringstream ss(inputLine);

      ss >> Time>> Buffer>> Bus>> MessageID>> Message>> MessageLength;
 
      obj.decode_message (MessageID, Message);  // speedID 180, steering angleID 37

        if (MessageID == 396){ 
          i++;
          obj.GetTrackAinfo() >> y >> x >> rel_spd;
       


        visualization_msgs::Marker marker;
        marker.header.frame_id = "front_laser_link";
        marker.header.stamp = ros::Time();
        marker.ns = "my_namespace";
        marker.id = 0;
        marker.type = 1;
        marker.action =0;
        marker.pose.position.x = x;
        marker.pose.position.y = y;
        marker.pose.position.z = 0.0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.2;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;
        marker.color.a = 1.0; // Don't forget to set the alpha!
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;

        pose_pub.publish(marker);
       rate.sleep();
      }
        
      
    }
    
      std::cout << "Finish publishing to the topic "<< std::endl;
     // inFile.close();
    

  return 0;
}






