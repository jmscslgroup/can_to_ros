/*
 Author: Matt Bunting
 Copyright (c) 2020 Arizona Board of Regents
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

#include <iostream>
// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Float64.h"
// Libpanda headers:
#include "panda/toyota.h"
#include <panda.h>


class Control {

private:
	// Initialize panda and toyota handlers
	Panda::Handler pandaHandler;
	Panda::ToyotaHandler toyotaHandler(&pandaHandler);
	ros::Subscriber sub_;

public:
Control(){

	ros::NodeHandle n_;
	ros::Subscriber sub_;
	// intializing a subscriber
	sub_ = n_.subscribe("/commands", 1000, &Control::callback, this);

	// Initialize panda and toyota handlers
	// Let's roll
	pandaHandler.initialize();
	toyotaHandler.start();
	// Setting HUD elements:
	// hudLaneLeft += mJoystickState.getButtonL1Rising();
	// hudLaneLeft -= mJoystickState.getButtonL2Rising();
	// hudLaneRight += mJoystickState.getButtonR1Rising();
	// hudLaneRight -= mJoystickState.getButtonR2Rising();
	// toyotaHandler.setHudLanes(hudLaneLeft, hudLaneRight);
	
	// toyotaHandler.setHudLdaAlert( mJoystickState.getTriangle() );
	// toyotaHandler.setHudTwoBeeps( mJoystickState.getX() );
	// toyotaHandler.setHudRepeatedBeeps( mJoystickState.getSelect() );
	// toyotaHandler.setHudBarrier( mJoystickState.getDY() > 0 );
	// toyotaHandler.setHudMiniCar( mJoystickState.getDX() > 0 );
	
	// // This will cancel the cruise control, cruise must be rest by driver to allow further controls
	// toyotaHandler.setHudCruiseCancelRequest( mJoystickState.getSquare() );
	
}

void callback(const std_msgs::Float64::ConstPtr& msg)
{
	// use these functions to set the acceleration and steeting Tourque
	toyotaHandler.setAcceleration(0.0);
	toyotaHandler.setSteerTorque(0.0);  // doesnt work yet

}
~Control(){
	// Will never reach here
	toyotaHandler.stop();
	pandaHandler.stop();
}

};


int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "send_commands");
	ROS_INFO("Initializing ..");
	Control vehicleControl;
    
    ros::spin();

  

    return 0;
}