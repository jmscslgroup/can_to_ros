
// /*This node is used to test the joystick input  in ROS */
// #include <iostream>
// #include <ros/ros.h>
// #include <sensor_msgs/Joy.h>



// void joyCallback(const sensor_msgs::Joy::ConstPtr& joy)

// {
// std::cout << "axes "<< joy->axes[0]<< std::endl;
// std::cout << "buttons A "<< joy->buttons[0]<< std::endl;
// std::cout << "buttons B "<< joy->buttons[1]<< std::endl;

// }



// int main(int argc, char** argv)
// {
//   ros::init(argc, argv, "joystick_testing");
//   ros::NodeHandle n;

//   ros::Subscriber sub = n.subscribe("joy", 10, joyCallback);


//   ros::spin();
// }