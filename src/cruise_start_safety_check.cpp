/*
 Author: Matt Bunting
 Hack Edits: Matt Nice
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
 This ROS node checks for cruise engage requests and will cancel the cruise controller if deemd to be unsafe.
 
 This expects to be using the vehicle_controller.launch file in
 
 Cruise will be engaged if and only if:
 - Upon a rising edge (false->true) on the topic /car/libpanda/controls_allowed, both:
	1) The topic /cmd_accel is withing a certain range [-0.5, 2.0]
	2) The topic /car/hud/mini_car_enable is true
 
 This node will only publish to topic /car/hud/cruise_cancel_request briefly on state changes to /car/libpanda/controls_allowed
 
 Expected node dependencies:
	- vehicle_interface.cpp (in vehicle_control.launch)
		- Subscribes to: /car/libpanda/controls_allowed
		- Publishes to: /car/hud/cruise_cancel_request
	- simple_mini_car_from_lead_distance.cpp
		- Subscribes to: /car/hud/mini_car_enable
	- Any other controller node
		- Subscribes to /cmd_accel
 
 Another option for detecting lead distance instead of checking /car/libpanda/mini_car_enable, but commented out:
	- subs_fs.cpp (to replace simple_mini_car_from_lead_distance.cpp)
		- Subscribes to: /lead_dist_869
 
 */


// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"

//#define CMD_ACCEL_MIN (-0.5) //this is the original more strict bound
#define CMD_ACCEL_MIN (-1.0) //HACK to make the engagement easier
#define CMD_ACCEL_MAX (2.0)	// value > 1.5 means unbounded

class CruiseStartSafetyCheck {
private:
//	ros::Subscriber subscriberLeadDistance;
	ros::Subscriber subscriberControlsAllowed;
	ros::Subscriber subscriberMiniCarEnable;
	ros::Subscriber subscriberCommandAcceleration;
	
	ros::Publisher publisherCruiseCancelRequest;
	
	bool lastControlsAllowed;
	bool leadVehicleVisible;
	bool commandedAccelerationWithinBounds;
	
public:
		
//	void callbackLeadDistance(const std_msgs::Float64::ConstPtr& msg) {
//		if(msg->data < 252) {
//			leadVehicleVisible = true;
//		} else {
//			leadVehicleVisible = false;
//		}
//	}
	
	void callbackMiniCarEnable(const std_msgs::Bool::ConstPtr& msg) {
		leadVehicleVisible = msg->data;
	}
	
	void callbackControlsAllowed(const std_msgs::Bool::ConstPtr& msg) {
		bool controlsAllowed = msg->data;
		
		// Only run on changes of value:
		if ( controlsAllowed != lastControlsAllowed ) {
			std_msgs::Bool cruiseCancelMessage;
			cruiseCancelMessage.data = true;
			
			// Check if the request state is valid:
			if (controlsAllowed &&                      // Rising edge
				commandedAccelerationWithinBounds){//HACKHACKHACK &&    // cmd_accel checks out
			//HACKHACKHACK	leadVehicleVisible ) {                  // Lead vehicle visible
				
				// If all the above checks out, no need to send a cancel request
				cruiseCancelMessage.data = false;
			}
			
			ROS_INFO("The states for this event are: controlsAllowed = %d, commandedAccelerationWithinBounds = %d, leadVehicleVisible = %d", controlsAllowed, commandedAccelerationWithinBounds, leadVehicleVisible);
			
			publisherCruiseCancelRequest.publish(cruiseCancelMessage);
			
			lastControlsAllowed = controlsAllowed;
		}
	}
	
	void callbackCommandAcceleration(const std_msgs::Float64::ConstPtr& msg) {
		double commandAcceleration = msg->data;
		if ((CMD_ACCEL_MIN <= commandAcceleration) && (commandAcceleration <= CMD_ACCEL_MAX)) {
			commandedAccelerationWithinBounds = true;
		} else {
			commandedAccelerationWithinBounds = false;
		}
	}
	
	CruiseStartSafetyCheck(ros::NodeHandle* nodeHandle) {
		lastControlsAllowed = false;
		leadVehicleVisible = false;
		commandedAccelerationWithinBounds = false;
		
		publisherCruiseCancelRequest = nodeHandle->advertise<std_msgs::Bool>("/car/hud/cruise_cancel_request", 1000);
		
//		subscriberLeadDistance = nodeHandle->subscribe("lead_dist_869", 1000, &CruiseStartSafetyCheck::callbackLeadDistance, this);
		subscriberMiniCarEnable = nodeHandle->subscribe("/car/hud/mini_car_enable", 1000, &CruiseStartSafetyCheck::callbackMiniCarEnable, this);
		subscriberControlsAllowed = nodeHandle->subscribe("/car/libpanda/controls_allowed", 1000, &CruiseStartSafetyCheck::callbackControlsAllowed, this);
		subscriberCommandAcceleration = nodeHandle->subscribe("/cmd_accel", 1000, &CruiseStartSafetyCheck::callbackCommandAcceleration, this);
	}
	
};



int main(int argc, char **argv) {
	ros::init(argc, argv, "cruise_start_safety_check");
	ROS_INFO("Waiting for controls_allowed state changes...");

	ros::NodeHandle nh;
	
	CruiseStartSafetyCheck mCruiseStartSafetyCheck(&nh);
	
    ros::spin();
	
    return 0;
}
