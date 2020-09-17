#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"



int main(int argc, char **argv){ 
ros::init(argc, argv, "director");
ros::NodeHandle nh1("~");

while (ros::ok())
    {
            
    }

ros::spin();
  return 0;
}