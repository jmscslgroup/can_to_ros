#include <iostream>
// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Float64.h"



class CtrlCommands {
 private:
    ros::NodeHandle n;
    std_msgs::Float64 mymsg;
    ros::Publisher command_pub;
    ros::Subscriber sub_;
    double receivedMsg = 0.0;


 public:
    CtrlCommands(){
        command_pub = n.advertise<std_msgs::Float64>("commands", 1000);
        sub_ = n.subscribe("/timed_accel", 1000, &CtrlCommands::callback, this);
    }

    void callback(const std_msgs::Float64::ConstPtr& msg){
        receivedMsg = msg->data;
    }

    void publishing(){
        mymsg.data=receivedMsg;
        command_pub.publish(mymsg);

    }


};


int main (int argc, char** argv ){

ros::init(argc, argv, "command_publisher");

CtrlCommands cmds;

ros::Rate loop_rate(100);

while (ros::ok())
  {
    cmds.publishing();
    ros::spinOnce();

    loop_rate.sleep();
  }
   
    return 0;
}