#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
// #include "can_to_ros/can_msgs.h"



int main(int argc, char **argv){ 
    ros::init(argc, argv, "publish_csv");
    ros::NodeHandle nh1("~");
    
    // ros::Publisher pub_ = nh1.advertise<can_to_ros::can_msgs>("/raw_data", 1000);
    ros::Publisher pub_ = nh1.advertise<std_msgs::String>("/realtime_raw_data", 1000);
    
    
    ROS_INFO("Got parameter : %s", argv[1]);
    
    std::ifstream inFile;
    std::ofstream outFile;
    std::string user_input="";
    std::string inputLine="";
    std::string Time,Buffer,Message,MessageLength;
    double MessageID, Bus;
    // values data;
    // decode_msgs obj;
    
    bool firstLine=true;
    if (argc != 2)
    { // check the nunber of the argument
        std::cout <<"./a.out .csv" << std::endl;
        return 1;
    }
    
    // outFile.open("headlights.csv");
    //    ros::Rate rate(2800.0); // the publish rate is 1/delta_t
    ros::Rate rate(10000.0); // This is abritray, just made VERY fast
    // outFile << "Time,"<< "LIGHT_STATE_CHANGED,"<< "HIGH_BEAMS_ON," << "HEAD_LAMPS_ON,"<< "RUNNING_LIGHTS_ON"<< std::endl;
    
    inFile.open(argv[1]);
    if( !inFile.is_open())
    {// check id the file is opened correctly.
        std::cout << "Cannot open file to read"<< std::endl;
        return 1;
    }
    
    std_msgs::String msgs;
    ros::Time startTime = ros::Time::now();
    ros::Time runningTime; // - current time - start time;
    ros::Time fileTime;
    ros::Time fileTimeStart;
    bool haveFileTimeStart = false;
    while (ros::ok()){  //ros::ok()
        
        if (!getline(inFile, inputLine))    // EOF
            break;
        
        if (firstLine){ // skip the first line
            firstLine=false;
            continue;
        }
        if (inputLine.empty()) continue; // if there is an empty line then skip it
        
        std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
        
        sscanf(inputLine.c_str(),"%d.%d", &fileTime.sec, &fileTime.nsec);
        
        fileTime.nsec *= 1000;
//        ROS_INFO("fileTime = %d.%09d", fileTime.sec, fileTime.nsec);
        if(!haveFileTimeStart) {
            fileTimeStart = fileTime;
            haveFileTimeStart = true;
        }
        msgs.data = inputLine;
        while( (ros::Time::now() - startTime).toSec() < (fileTime - fileTimeStart).toSec() ) {
            rate.sleep();
        }
        pub_.publish(msgs);
        
        
        //rate.sleep();
        
    }
    
    std::cout << "Finish publishing raw data "<< std::endl;
    
    //  ros::spin();
    return 0;
}
