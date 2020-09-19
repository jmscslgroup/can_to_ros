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
    ros::Publisher pub_ = nh1.advertise<std_msgs::String>("/realtime_raw_data", 1000);


    ROS_INFO("Got parameter : %s", argv[1]);
  
    std::ifstream inFile;
    std::ofstream outFile;
    std::string user_input="";
    std::string inputLine="";
    // std::string Time,Buffer,Message,MessageLength;
    // int MessageID, Bus;
    // values data;
    // decode_msgs obj;

    bool firstLine=true;
    if (argc != 2)
    { // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }

// outFile.open("headlights.csv");
    ros::Rate rate(2800.0); // the publish rate is 1/delta_t 
// outFile << "Time,"<< "LIGHT_STATE_CHANGED,"<< "HIGH_BEAMS_ON," << "HEAD_LAMPS_ON,"<< "RUNNING_LIGHTS_ON"<< std::endl;

    inFile.open(argv[1]);
    if( !inFile.is_open())
    {// check id the file is opened correctly.
      std::cout << "Cannot open file to read"<< std::endl;
      return 1;
    }

    while (ros::ok()){  //ros::ok()
        
        if (!getline(inFile, inputLine)) break;
        if (firstLine){ // skip the first line
            firstLine=false;
            continue;
        }
        if (inputLine.empty()) continue; // if there is an empty line then skip it

        std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
        // std::stringstream ss(inputLine);
        // ss >> Time>> Bus>> MessageID>> Message>> MessageLength;
        
        // if (MessageID == 1570 && Bus ==0){
        // data = obj.decode_message (MessageID, Message);

        // outFile << Time << "," << data.var1<< ","<< data.var2 <<"," << data.var3 <<","<< data.var4 << std::endl;
        // }

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

//  ros::spin();
  return 0;
}