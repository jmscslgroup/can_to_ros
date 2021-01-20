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
// #include "std_msgs/String.h"
// #include "geometry_msgs/Point.h"
// #include "geometry_msgs/Twist.h"
// #include "geometry_msgs/PointStamped.h"
// #include "sensor_msgs/TimeReference.h"
// #include "header_package/can_decode.h"
// #include "visualization_msgs/Marker.h"
// Libpanda headers:
#include "panda/toyota.h"

int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "send_commands");
	ROS_INFO("Initializing ..");

    // Initialize panda and toyota handlers
	Panda::Handler pandaHandler;
	Panda::ToyotaHandler toyotaHandler(&pandaHandler);
	
	// Let's roll
	pandaHandler.initialize();
	toyotaHandler.start();
    //publish rate 
    ros::Rate rate(10.0); 

    while (ros::ok())
    {  
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

        // use these functions to set the acceleration and steeting Tourque
        toyotaHandler.setAcceleration(0.0);
		toyotaHandler.setSteerTorque(0.0);  // doesnt work yet

        ros::spin();
	    rate.sleep();    

    }


    // Will never reach here
	toyotaHandler.stop();
	pandaHandler.stop();

    return 0;
}