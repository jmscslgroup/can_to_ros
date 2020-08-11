#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/TwistStamped.h"


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    accel_pub = n_.advertise<geometry_msgs::AccelStamped>("/vehicle/accel", 1000);  
    lead_dist_pub = n_.advertise<geometry_msgs::PointStamped>("/vehicle/distanceEstimator/dist", 1000);
    str_angle_pub = n_.advertise<geometry_msgs::PointStamped>("/vehicle/steering_angle", 1000);
    speed_pub = n_.advertise<geometry_msgs::TwistStamped>("/vehicle/vel", 1000);

    //Topic you want to subscribe
    sub_ = n_.subscribe("/raw_data", 1000, &SubscribeAndPublish::callback, this);
  }

  void callback(const std_msgs::String::ConstPtr& raw_data)
  {
    std::stringstream ss(raw_data->data);
    ss >> Time>> Bus>> MessageID>> Message>> MessageLength;
    
    if(MessageID == 869 && Bus == 0 )
    {
     data = obj.decode_message (MessageID, Message);
     geometry_msgs::PointStamped dist;
     dist.header.stamp=ros::Time(std::stod(Time));
     dist.point.x = data.var1;
     lead_dist_pub.publish(dist);

    }
    else if (MessageID == 552 && Bus==0)
    { 
      data = obj.decode_message (MessageID, Message);
      geometry_msgs::AccelStamped msg;
      msg.header.stamp=ros::Time(std::stod(Time));
      msg.accel.linear.x = data.var1;
      accel_pub.publish(msg);
      
    }
    else if (MessageID == 37 && Bus == 0)
    { 
       data = obj.decode_message (MessageID, Message);
       geometry_msgs::PointStamped str_angle;
       str_angle.header.stamp=ros::Time(std::stod(Time));
       str_angle.point.x = data.var1;
       str_angle_pub.publish(str_angle);

    }
    else if (MessageID == 180 && Bus == 0)
    { 
       data = obj.decode_message (MessageID, Message); 
       geometry_msgs::TwistStamped msg;
       msg.header.stamp=ros::Time(std::stod(Time));
       msg.twist.linear.x = data.var1;
       speed_pub.publish(msg);
       }
    

}

private:
  ros::NodeHandle n_;
  ros::Publisher accel_pub;
  ros::Publisher lead_dist_pub;
  ros::Publisher str_angle_pub;
  ros::Publisher speed_pub;
  ros::Subscriber sub_;
  decode_msgs obj;
  std::string Time,Buffer,Message,MessageLength;
  int MessageID, Bus;
  values data;

};//End of class SubscribeAndPublish

// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "subs");
    ros::NodeHandle nh1;
    ROS_INFO("Got parameter");

    SubscribeAndPublish SAPObject;
     ros::spin();   

  return 0;
}