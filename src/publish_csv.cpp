#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "can_to_ros/can_msgs.h"



int main(int argc, char **argv){ 
    ros::init(argc, argv, "my_node");
    ros::NodeHandle nh1("~");
 
    // ros::Publisher pub_ = nh1.advertise<can_to_ros::can_msgs>("/raw_data", 1000);
    ros::Publisher pub_ = nh1.advertise<std_msgs::String>("/raw_data", 1000);


    ROS_INFO("Got parameter : %s", argv[1]);
  
    std::ifstream inFile;
    std::string user_input="";
    std::string inputLine="";
    int MessageID;

    bool firstLine=true;
    if (argc != 2)
    { // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }

    ros::Rate rate(2620.0); // the publish rate is 1/delta_t 

    inFile.open(argv[1]);
    if( !inFile.is_open())
    {// check id the file is opened correctly.
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

        // can_to_ros::can_msgs msgs;
        // msgs.timestamp=0.00;
        // msgs.bus=1;
        // msgs.messageid=180;
        // msgs.candata="fdgs5456466";
        // msgs.datalength=20;

        std_msgs::String msgs;
        msgs.data=inputLine;
        pub_.publish(msgs);
        rate.sleep();    

    }

    std::cout << "Finish publishing raw data "<< std::endl;   

 ros::spin();
  return 0;
}