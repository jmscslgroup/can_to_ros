#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"



int main(int argc, char **argv){ 
    ros::init(argc, argv, "realtime_publisher");
    ros::NodeHandle nh1("~");
 
    ros::Publisher pub_ = nh1.advertise<std_msgs::String>("/raw_data", 1000);


    ROS_INFO("Got parameter : %s", argv[1]);
  
    std::ifstream inFile;
    std::string user_input="";
    std::string inputLine="";
    int MessageID;

    // bool firstLine=true;
    // if (argc != 2)
    // { // check the nunber of the argument
    //     std::cout <<"./a.out .csv" << std::endl;
    //     return 1;
    // }

    ros::Rate rate(2620.0); // the publish rate is 1/delta_t 

    // inFile.open(argv[1]);
    // if( !inFile.is_open())
    // {// check id the file is opened correctly.
    //   std::cout << "Cannot open file to read"<< std::endl;
    //   return 1;
    // }

    while (ros::ok()){
        
     if (!getline(std::cin, inputLine)) break;

     if (inputLine.empty()) continue; // if there is an empty line then skip it

      std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
        std_msgs::String msgs;
        msgs.data=inputLine;
        pub_.publish(msgs);
        rate.sleep();    

    }

    std::cout << "Finish publishing raw data "<< std::endl;   

 ros::spin();
  return 0;
}