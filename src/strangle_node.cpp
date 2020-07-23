#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
/**********************************************************************************************/

int main(int argc, char **argv){
    ros::init(argc, argv, "strangle_node");
    ros::NodeHandle nh("~");
    ros::Publisher str_angle_pub = nh.advertise<geometry_msgs::PointStamped>("/vehicle/steering_angle", 1000);   // pulishing to /vehicle/vel topic
    ROS_INFO("Got parameter : %s", argv[1]);
    std::ifstream inFile;
    decode_msgs obj;
    std::string inputLine="";
    std::string Time,Buffer,Message,MessageLength;
    int MessageID, Bus;
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

      if (MessageID == 37 && Bus == 0){ 
      //  std::cout << "str angle " << obj.GetSteeringAngle() << std::endl;
       data = obj.decode_message (MessageID, Message);
       geometry_msgs::PointStamped str_angle;
       str_angle.header.stamp=ros::Time(std::stod(Time));
       str_angle.point.x = data.var1;
        str_angle_pub.publish(str_angle);
      // ros::spinOnce();
        rate.sleep();
      }

    }
     
      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
