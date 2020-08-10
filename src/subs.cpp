#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"



class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    pub_ = n_.advertise<geometry_msgs::PointStamped>("/test_topic", 1000);
    pub_1=n_.advertise<geometry_msgs::PointStamped>("/test_topic1", 1000);
    pub_2=n_.advertise<geometry_msgs::PointStamped>("/test_topic2", 1000);


    //Topic you want to subscribe
    sub_ = n_.subscribe("/raw_data", 1000, &SubscribeAndPublish::callback, this);
  }

  void callback(const std_msgs::String::ConstPtr& raw_data)
  {
    decode_msgs obj;
    std::string Time,Buffer,Message,MessageLength;
    int MessageID, Bus;
    values data;
    std::stringstream ss(raw_data->data);
    ss >> Time>> Bus>> MessageID>> Message>> MessageLength;
    geometry_msgs::PointStamped msg;
    
    if(MessageID == 869 && Bus == 0 ){
    data = obj.decode_message (MessageID, Message);
    msg.header.stamp=ros::Time(std::stod(Time));
    msg.point.x=data.var1;
    pub_.publish(msg);

   // ROS_INFO("I heard: [%s]", raw_data->data.c_str());
    }
        if(MessageID == 400 ){
    data = obj.decode_message (MessageID, Message);
    msg.header.stamp=ros::Time(std::stod(Time));
    msg.point.x=data.var2;
    pub_1.publish(msg);

   // ROS_INFO("I heard: [%s]", raw_data->data.c_str());
    }
    if(MessageID == 402 ){
    data = obj.decode_message (MessageID, Message);
    msg.header.stamp=ros::Time(std::stod(Time));
    msg.point.x=data.var2;
    pub_2.publish(msg);

   // ROS_INFO("I heard: [%s]", raw_data->data.c_str());
    }
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Publisher pub_1;
  ros::Publisher pub_2;

  ros::Subscriber sub_;

};//End of class SubscribeAndPublish


    // ros::NodeHandle n;
    // ros::Publisher pose_pub =  n.advertise<geometry_msgs::PointStamped>("/test_topic", 1000);

// void subsCallback(const std_msgs::String::ConstPtr& raw_data)
// {



// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "subs");
    ros::NodeHandle nh1;
    ROS_INFO("Got parameter");

    
    // ros::Rate loop_rate(20);
    SubscribeAndPublish SAPObject;
    // while (ros::ok())
    // {
    
    // // ros::Subscriber sub = nh1.subscribe("/raw_data", 1000, subsCallback);

    
    // ros::Rate loop_rate(0.1);
    // ros::spinOnce();
    // }
     ros::spin();   
  
 
  return 0;
}