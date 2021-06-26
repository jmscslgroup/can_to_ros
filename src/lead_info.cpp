#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/Twist.h"
//#include "can_to_ros/headlights.h"
#include "std_msgs/UInt8.h"
#include <cstdlib>



class LeadInfo
{
public:
  LeadInfo()
  {
    //Topic you want to publish
    relative_vel_pub = n_.advertise<geometry_msgs::Twist>("rel_vel", 1000);

    //Topic you want to subscribe
    tracka0_sub = n_.subscribe("track_a0", 100, &LeadInfo::callback, this);
    tracka1_sub = n_.subscribe("track_a1", 100, &LeadInfo::callback, this);
    tracka2_sub = n_.subscribe("track_a2", 100, &LeadInfo::callback, this);
    tracka3_sub = n_.subscribe("track_a3", 100, &LeadInfo::callback, this);
    tracka4_sub = n_.subscribe("track_a4", 100, &LeadInfo::callback, this);
    tracka5_sub = n_.subscribe("track_a5", 100, &LeadInfo::callback, this);
    tracka6_sub = n_.subscribe("track_a6", 100, &LeadInfo::callback, this);
    tracka7_sub = n_.subscribe("track_a7", 100, &LeadInfo::callback, this);
    tracka8_sub = n_.subscribe("track_a8", 100, &LeadInfo::callback, this);
    tracka9_sub = n_.subscribe("track_a9", 100, &LeadInfo::callback, this);
    tracka10_sub = n_.subscribe("track_a10", 100, &LeadInfo::callback, this);
    tracka11_sub = n_.subscribe("track_a11", 100, &LeadInfo::callback, this);
    tracka12_sub = n_.subscribe("track_a12", 100, &LeadInfo::callback, this);
    tracka13_sub = n_.subscribe("track_a13", 100, &LeadInfo::callback, this);
    tracka14_sub = n_.subscribe("track_a14", 100, &LeadInfo::callback, this);
    tracka15_sub = n_.subscribe("track_a15", 100, &LeadInfo::callback, this);
  }

  void callback(const geometry_msgs::PointStamped::ConstPtr& radar)
  {
      std::cout << "long: " << radar->point.x << std::endl; 
      std::cout << "lat: " << radar->point.y << std::endl; 
      if (abs(radar->point.y) <= 0.5){
          geometry_msgs::Twist msg;
          msg.linear.x = radar->point.x; //long 
          msg.linear.y = radar->point.y; //lat
          msg.linear.z = radar->point.z; // rel_v
          relative_vel_pub.publish(msg);

      }
    }

private:
  ros::NodeHandle n_;
  ros::Publisher relative_vel_pub;

  ros::Subscriber tracka0_sub;
  ros::Subscriber tracka1_sub;
  ros::Subscriber tracka2_sub;
  ros::Subscriber tracka3_sub;
  ros::Subscriber tracka4_sub;
  ros::Subscriber tracka5_sub;
  ros::Subscriber tracka6_sub;
  ros::Subscriber tracka7_sub;
  ros::Subscriber tracka8_sub;
  ros::Subscriber tracka9_sub;
  ros::Subscriber tracka10_sub;
  ros::Subscriber tracka11_sub;
  ros::Subscriber tracka12_sub;
  ros::Subscriber tracka13_sub;
  ros::Subscriber tracka14_sub;
  ros::Subscriber tracka15_sub;


};//End of class 

// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "lead_info");
    ros::NodeHandle nh1;

    LeadInfo relative_velocity;
     ros::spin();   

  return 0;
}
