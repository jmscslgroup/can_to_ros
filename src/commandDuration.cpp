#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <stdlib.h>


int main(int argc, char **argv)
{

  ros::init(argc, argv, "command_duration");

  ros::NodeHandle n;

  // Advertize the publisher on the topic you like
  ros::Publisher pub = n.advertise<std_msgs::Float64 >("timed_accel", 10);
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::Float64 accel_msg;

    // Here you build your twist message
    accel_msg.data=1.0;   // setting the accelaration value
    // ros::Duration(5).sleep();
    ros::Time beginTime = ros::Time::now();
    ros::Duration secondsIWantToSendMessagesFor = ros::Duration(3.0); 
    ros::Time endTime = beginTime + secondsIWantToSendMessagesFor;
    
    while (ros::Time::now() < endTime )
    {
        pub.publish(accel_msg);

        // Time between messages, so you don't blast out an thousands of 
        // messages in your 3 secondperiod
        ros::Duration(0.1).sleep();
    }
    std::cout<< "Finished sending accel command." << std::endl;

    accel_msg.data=0.0;
    endTime=ros::Time::now() + ros::Duration(1.0);
     while (ros::Time::now() < endTime)
    {
        pub.publish(accel_msg);

        // Time between messages, so you don't blast out an thousands of 
        ros::Duration(0.1).sleep();
    }

  return 0;
}