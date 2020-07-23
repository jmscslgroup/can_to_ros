#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/NavSatFix.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "sensor_msgs/TimeReference.h"
#include <time.h>



/**********************************************************************************************/
int main(int argc, char **argv){
    std::ifstream inFile;
    std::string Gpstime,Status,Long,Lat,Alt,HDOP,PDOP,VDOP;
    ros::init(argc, argv, "gpsfix");
    ros::NodeHandle nh("~");
    ros::Publisher gps_pub = nh.advertise<sensor_msgs::NavSatFix>("/vehicle/gpsfix", 1000);   // pulishing to /vehicle/vel topic
    // ros::Publisher gps_time_pub = nh.advertise<sensor_msgs::TimeReference>("/vehicle/gpstime", 1000);   // pulishing to /vehicle/vel topic


    std::string inputLine;
  
    bool firstLine=true;
    if (argc != 2){ // check the nunber of the argument
        std::cout <<"./a.out GPS.csv" << std::endl;
        return 1;
    }
  
    inFile.open(argv[1]);
    if( !inFile.is_open()){// check id the file is opened correctly.
      std::cout << "Cannot open file to read"<< std::endl;
      return 1;
    }


      ros::Rate rate(20.0); // the publish rate is 1/delta_t 
    while (ros::ok()){

      if (!getline(inFile, inputLine)) break;
      if (firstLine){ // skip the first line
        firstLine=false;
        continue;
      }
     if (inputLine.empty()) continue; // if there is an empty line then skip it

      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
      std::stringstream ss(inputLine);
      ss >> Gpstime>>Status>>Long>>Lat>>Alt>>HDOP>>PDOP>>VDOP;
      if (Status == "V") continue;

      sensor_msgs::NavSatFix gps_msg;

      gps_msg.header.frame_id = "";
      gps_msg.header.stamp =ros::Time(std::stod(Gpstime));
      gps_msg.latitude=std::stod(Lat);
      gps_msg.longitude=std::stod(Long);
      gps_msg.altitude=std::stod(Alt);
      gps_msg.position_covariance[0]=3.9561210000000004;
      gps_msg.position_covariance[1]=0.0;
      gps_msg.position_covariance[2]=0.0;
      gps_msg.position_covariance[3]=0.0;
      gps_msg.position_covariance[4]= 3.9561210000000004;
      gps_msg.position_covariance[5]=0.0;
      gps_msg.position_covariance[6]=0.0;
      gps_msg.position_covariance[7]=0.0;
      gps_msg.position_covariance[8]=7.650756;
      gps_msg.position_covariance_type= 2;
      
      // adding a Time refrence message to tell us the atual time of gps messages
      // sensor_msgs::TimeReference gps_time_ref_msg;
      // gps_time_ref_msg.source = "gps";
      // gps_time_ref_msg.header.stamp = ros::Time::now();
      // //ros::Time::now();
      // gps_time_ref_msg.time_ref = ros::Time(std::stod(Gpstime));
      // gps_time_pub.publish(gps_time_ref_msg);

      gps_pub.publish(gps_msg);

        rate.sleep();
      }
  
      std::cout << "Finish publishing to the topic "<< std::endl;
    

  return 0;
}