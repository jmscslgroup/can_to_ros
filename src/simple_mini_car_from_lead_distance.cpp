/*
 Author: Matt Bunting
 Copyright (c) 2021 Arizona Board of Regents
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in
 all copies of this software.
 IN NO EVENT SHALL THE ARIZONA BOARD OF REGENTS BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.
 THE ARIZONA BOARD OF REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER
 IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION
 TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 */

/*
 This ROS node takes the lead distance and if a car is seen, enabled the vehicle's "Mini Car" HUD
 
 Details:
 1) This is expected to use the node can_to_ros subs_fs.cpp to produce the lead_dist_869 message of type std_msgs/Float64
 
 2) A comparison is performed:  If the lead distance is less than 252, the radar sees a lead vehicle.
 
 3) This publishes a message to can_to_ros vehicle_interface.cpp /car/hud/mini_car_enable of type std_msgs/Bool to display the mini car
 
 */


// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"
#include "header_package/can_decode.h"


class SimpleLeadDistanceToMiniCar {
private:
	ros::Subscriber subscriberLeadDistance;
	ros::Publisher publisherMiniCarHud;
	
public:
	void callback(const std_msgs::Float64::ConstPtr& msg)
	{
		std_msgs::Bool miniCarEnable;
		miniCarEnable.data = false;
		if(msg->data < 252) {
			miniCarEnable.data = true;
		}
		publisherMiniCarHud.publish(miniCarEnable);
	}
	
	SimpleLeadDistanceToMiniCar(ros::NodeHandle* nodeHandle) {
		publisherMiniCarHud = nodeHandle->advertise<std_msgs::Bool>("/car/hud/mini_car_enable", 1000);
		subscriberLeadDistance = nodeHandle->subscribe("lead_dist_869", 1000, &SimpleLeadDistanceToMiniCar::callback, this);
	}
	
};



int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "simple_mini_car_from_lead_distance");
	ROS_INFO("Initializing...");

	ros::NodeHandle nh;
	
    // Initialize Libpanda with ROS publisher:
	SimpleLeadDistanceToMiniCar mSimpleLeadDistanceToMiniCar(&nh);
	
    ros::spin();
	
    return 0;
}
