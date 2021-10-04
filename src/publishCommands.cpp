#include <iostream>
// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Float64.h"



class CtrlCommands {
 private:
    ros::NodeHandle n;
    std_msgs::Float64 mymsg;
    std_msgs::Float64 steermsg;
    ros::Publisher command_pub;
    ros::Publisher steer_pub;
    ros::Subscriber sub_0;
    ros::Subscriber sub_1;

    double receivedMsg = 0.0;

//
 public:
    CtrlCommands(){
//        command_pub = n.advertise<std_msgs::Float64>("commands", 1000);
		command_pub = n.advertise<std_msgs::Float64>("car/cruise/accel_input", 1000);
    steer_pub = n.advertise<std_msgs::Float64>("car/cruise/steer_input",500);
    sub_0 = n.subscribe("/timed_accel", 1000, &CtrlCommands::callback, this);
//        sub_1 = n.subscribe("/cmd_accel", 1000, &CtrlCommands::callback, this);
		sub_1 = n.subscribe("/cmd_accel_safe", 1000, &CtrlCommands::callback, this);

    }

    void callback(const std_msgs::Float64::ConstPtr& msg){
        receivedMsg = msg->data;
    }

    void publishing(){
        mymsg.data=receivedMsg;
        command_pub.publish(mymsg);
        steermsg.data == 10;
        steer_pub.publish(steermsg);

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
