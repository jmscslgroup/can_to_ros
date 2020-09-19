#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"

//SUBS AND PUBS
//want to subscribe to the modeChanger topic
//want to publish the mode [0,1,2,3,4] topic (or should it be parameter?)
//want to publish the time headway set point [1,2,3] (this is for dth mode)
//want to publish the binary for sound [0,1] (so CANCoach can stay on but silent)

//OTHER
//start in mode 0
//proceed to mode 1 from 'next' modeChanger signal
//each non-zero mode has a timer:
	//e.g. CTH, there's 'Normal'->'Instructed'->'Coached'
	//so mode=1->1->1,setpoint=3->3->3,sound=0->0->1
	//will automatically progress to the next subsection settings on a timer
	//
//want to play the .wav files directing the driver on what to do



int main(int argc, char **argv){ 
	ros::init(argc, argv, "director");
	ros::NodeHandle nh1("~");
	ROS_INFO("Starting Director")

	while (ros::ok())
	{
            
    	}

	ros::spin();
	return 0;
}
