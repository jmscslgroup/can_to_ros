#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"

int main(int argc, char **argv){
    ros::init(argc, argv, "track_a0");
     ros::NodeHandle nh1("~");
 
    ros::Publisher pose_pub = nh1.advertise<visualization_msgs::Marker>("/track_a0", 1000);

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

        if (MessageID == 384){ 
        data = obj.decode_message (MessageID, Message);
        visualization_msgs::Marker marker;
        marker.header.frame_id = "front_laser_link";
        marker.header.stamp = ros::Time(std::stod(Time));
        marker.ns = "my_namespace";
        marker.id = 0;
        marker.type = 1;
        marker.action =0;
        marker.pose.position.x = data.var2; // 
        marker.pose.position.y = (data.var1 * -1.0); // 
        // marker.pose.position.x = 5.0;
        // marker.pose.position.y = -2.0;
        marker.pose.position.z = 0.0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w =1.0;
        marker.scale.x = 0.2;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;
        marker.color.a = 1.0; // Don't forget to set the alpha!
        if (data.var2 <= 2.0)
        {
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;
        }
        else
        {
        marker.color.r = 1.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        }

        std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
        struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
        // struct tm is structure holding a calendar date and time broken down into its components.
        int hour= ptm->tm_hour;
        int min= ptm->tm_min;
        int sec= ptm->tm_sec;
        std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

        pose_pub.publish(marker);
       rate.sleep();
      }
        
    }
    
      std::cout << "Finish publishing to the topic "<< std::endl;
 
  return 0;
}