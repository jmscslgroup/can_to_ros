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

  double r_lat = 0.0;
  double r_long = 0.0;
  double r_velocity = 0.0;
  double relative_velocity = 0.0;
  double lead_distance = 0.0;
  double last_read_lead_dist = 0.0;
  LeadInfo()
  {
    //Topic you want to publish
    relative_vel_pub = n_.advertise<geometry_msgs::Twist>("rel_vel", 1000);
    lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist", 1000);


    leaddist_sub = n_.subscribe("lead_dist_869", 100, &LeadInfo::callback_lead, this);

    rel_vel_sub = n_.subscribe("rel_vel_869", 100, &LeadInfo::callback_rel_vel, this);


    //Topic you want to subscribe
  //   tracka0_sub = n_.subscribe("track_a0", 100, &LeadInfo::callback, this);
  //   tracka1_sub = n_.subscribe("track_a1", 100, &LeadInfo::callback, this);
  //   tracka2_sub = n_.subscribe("track_a2", 100, &LeadInfo::callback, this);
  //   tracka3_sub = n_.subscribe("track_a3", 100, &LeadInfo::callback, this);
  //   tracka4_sub = n_.subscribe("track_a4", 100, &LeadInfo::callback, this);
  //   tracka5_sub = n_.subscribe("track_a5", 100, &LeadInfo::callback, this);
  //   tracka6_sub = n_.subscribe("track_a6", 100, &LeadInfo::callback, this);
  //   tracka7_sub = n_.subscribe("track_a7", 100, &LeadInfo::callback, this);
  //   tracka8_sub = n_.subscribe("track_a8", 100, &LeadInfo::callback, this);
  //   tracka9_sub = n_.subscribe("track_a9", 100, &LeadInfo::callback, this);
  //   tracka10_sub = n_.subscribe("track_a10", 100, &LeadInfo::callback, this);
  //   tracka11_sub = n_.subscribe("track_a11", 100, &LeadInfo::callback, this);
  //   tracka12_sub = n_.subscribe("track_a12", 100, &LeadInfo::callback, this);
  //   tracka13_sub = n_.subscribe("track_a13", 100, &LeadInfo::callback, this);
  //   tracka14_sub = n_.subscribe("track_a14", 100, &LeadInfo::callback, this);
  //   tracka15_sub = n_.subscribe("track_a15", 100, &LeadInfo::callback, this);
  // }

  // void callback(const geometry_msgs::PointStamped::ConstPtr& radar)
  // {
  //     // std::cout << "long: " << radar->point.x << std::endl; 
  //     // std::cout << "lat: " << radar->point.y << std::endl; 
  //         // 20+10=30                15                20-10=10         15 
  //   if ( lead_distance +1 >= radar->point.x && lead_distance -1 <= radar->point.x && radar->point.x < 252)  {
  //     if (abs(radar->point.y) <= 0.5){
  //         last_read_lead_dist = radar->point.x;
  //         r_lat = radar->point.y;
  //         r_long = radar->point.x;
  //         r_velocity =  radar->point.z;
  //         geometry_msgs::Twist msg;
  //         std_msgs::Float64 dist;
  //         msg.linear.x = radar->point.x; //long 
  //         msg.linear.y = radar->point.y; //lat
  //         msg.linear.z = radar->point.z; // rel_v
  //         relative_vel_pub.publish(msg);   
  //         dist.data = radar->point.x;
  //         lead_dist_pub.publish(dist);


  //     }
  //   }
  //   else if (last_read_lead_dist +1 >= radar->point.x && last_read_lead_dist -1 <= radar->point.x && radar->point.x < 252){

  //     if (abs(radar->point.y) <= 0.5){
  //       last_read_lead_dist = radar->point.x;
  //       r_lat = radar->point.y;
  //       r_long = radar->point.x;
  //       r_velocity =  radar->point.z;
  //       geometry_msgs::Twist msg;
  //       std_msgs::Float64 dist;
  //       msg.linear.x = radar->point.x; //long 
  //       msg.linear.y = radar->point.y; //lat
  //       msg.linear.z = radar->point.z; // rel_v
  //       relative_vel_pub.publish(msg);
  //       dist.data = radar->point.x;
  //       lead_dist_pub.publish(dist);

  //     }
  //   }
  }


      void callback_lead(const std_msgs::Float64::ConstPtr& dist)
  {
      lead_distance = dist->data;
      std_msgs::Float64 msg;
      msg.data = lead_distance;
      lead_dist_pub.publish(msg); 

 
    }

    void callback_rel_vel(const std_msgs::Float64::ConstPtr& relvel)
  {
      relative_velocity = relvel->data; 
      geometry_msgs::Twist msg;
      msg.linear.x = 0.0; //long 
      msg.linear.y = 0.0; //lat
      msg.linear.z = relative_velocity; // rel_v from 869
      relative_vel_pub.publish(msg);
 
    }

    

private:
  ros::NodeHandle n_;
  ros::Publisher relative_vel_pub;
  ros::Publisher lead_dist_pub;

  ros::Subscriber leaddist_sub;
  ros::Subscriber rel_vel_sub;

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

    LeadInfo relative_velocity_obj;
     ros::spin();   

  return 0;
}
