#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"

int main(int argc, char **argv){
    ros::init(argc, argv, "track_a8");
     ros::NodeHandle nh1("~");
 
    ros::Publisher pose_pub = nh1.advertise<visualization_msgs::Marker>("/track_a8", 1000);

    ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    std::string user_input="";
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Buffer,Bus,Message,MessageLength;
    int MessageID;
    float y,x,rel_spd;
    float az;
    int i=0;
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

        if (MessageID == 392){ 
        data = obj.decode_message (MessageID, Message);
        visualization_msgs::Marker marker;
        marker.header.frame_id = "front_laser_link";
        marker.header.stamp = ros::Time(std::stod(Time));
        marker.ns = "my_namespace";
        marker.id = 0;
        marker.type = 1;
        marker.action =0;
        marker.pose.position.x = data.var1;
        marker.pose.position.y = data.var2;
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
 
  return 0;
}