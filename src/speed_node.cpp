#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"

/**********************************************************************************************/

int main(int argc, char **argv){
    ros::init(argc, argv, "speed_node");
    ros::NodeHandle nh("~");
    ros::Publisher speed_pub = nh.advertise<geometry_msgs::Twist>("/vehicle/vel", 1000);   // pulishing to /vehicle/vel topic 
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
 

      if (MessageID == 180){ 
        data = obj.decode_message (MessageID, Message); 
      // std::cout << "speed " << obj.GetSpeed() << std::endl;
       geometry_msgs::Twist msg;
       msg.linear.x = data.var1;
       speed_pub.publish(msg);
       ros::spinOnce();
       rate.sleep();
       
       }

    }
     
      std::cout << "Finish publishing to the topic "<< std::endl;
      inFile.close();

  return 0;
}
