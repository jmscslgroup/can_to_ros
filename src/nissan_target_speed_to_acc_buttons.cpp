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
#include <stdlib.h>
#include <fstream>
// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"

#include <sstream>

/*
 This ROS node interfaces libpanda's ToyotaHandler
 
 Publishers:
 1) /realtime_raw_data - std_msgs/String -  This publishes CAN data of interest where the can_to_ros node named subs_fs.cpp can interpret values
 2) /car/panda/gas_interceptor_detected - std_msgs/Bool - Reported by the Panda is their gas interceptor hardware is detected
 3) /car/panda/controls_allowed - std_msgs/Bool -  Reported by the comma.ai panda.  This is not event-based from the Panda, but is regularly checked at 2 Hz to reset the Panda's heartbeat
 4)	/car/libpanda/controls_allowed - std_msgs/Bool -  Reported by logic within libpanda.  This is event based from libpanda using CAN messages.  When no events occur, this regularly published at 1 Hz which can be used to assess libpanda's control command health
 
 Subscribers:
 1) /car/cruise/accel_input - std_msgs/Float64 - This is for acceleration commands to be sent to the car's cruise controller (priorly known as /commands)
 2) /car/hud/mini_car_enable - std_msgs/Bool - When true, this will display a mini-vehicle on the car's HUD which cruise control is on and engaged
 3) /car/hud/cruise_cancel_request - std_msgs/Bool - When true  published, the cruise controller will disengage and notify the driver with an audible chime
 
 */


// copied from libapnda buttonSequence.h, which should be included here but oh well
enum NissanButton {
	NISSAN_BUTTON_CRUISE_ON = 5,	// "system toggle"
	NISSAN_BUTTON_CANCEL = 4,
	NISSAN_BUTTON_DISTANCE = 3,
	NISSAN_BUTTON_SET = 2,
	NISSAN_BUTTON_RES = 1,
	NISSAN_BUTTON_OFF = 0,
};


// State definitions
typedef enum {
	STATE_CONTROLS_OFF,
	// The rest represent CONTROLS_ON
	STATE_IDLE,	// ON
	STATE_SHORT_PRESS,
	STATE_TURN_OFF,
	STATE_LONG_PRESS,
	STATE_CHECK_HOLD,
	//ACC_STATE_COME_HOME	// ON
} CommandState;


#define PROCESS_RATE (20.0)	// Hz

#define BUTTON_PRESS_TIME (0.25)	// seconds
#define BUTTON_LONG_PRESS_TIME (0.5)	// seconds
#define BUTTON_RELEASE_TIME (0.5)	// seconds

#define PROCESS_DECIMATOR_PRESS (PROCESS_RATE * BUTTON_PRESS_TIME)
#define PROCESS_DECIMATOR_LONG_PRESS (PROCESS_RATE * BUTTON_LONG_PRESS_TIME)
#define PROCESS_DECIMATOR_RELEASE (PROCESS_RATE * BUTTON_RELEASE_TIME)


class SetPointToAccButtonLogic {
private:
	// ROS stuff:
	ros::NodeHandle* nodeHandle;
	ros::Publisher publisherButtonRequest;
	ros::Subscriber subscriberTargetSpeed;
	ros::Subscriber subscriberTargetDistanceSetting;
	ros::Subscriber subscriberSetSpeed;
	ros::Subscriber subscriberSetDistance;
	ros::Subscriber subscriberControlsAllowed;
	
	// State variables
	short target_speed;
	unsigned char target_distance_setting;//uint8
	short set_speed;
	short set_distance;//int16
	bool controls_allowed;
	
	CommandState state;
	
	
	// Helper variables:
	NissanButton buttonToSend;
	
	// Decimators for timing:
	int decimatorShortPress;
	int decimatorLongPress;
	int decimatorRelease;
	
	
	const char* stateToName( const CommandState& state) {
		switch (state) {
			case STATE_CONTROLS_OFF: return "STATE_CONTROLS_OFF";
			case STATE_IDLE: return "STATE_IDLE";
			case STATE_SHORT_PRESS: return "STATE_SHORT_PRESS";
			case STATE_TURN_OFF: return "STATE_TURN_OFF";
			case STATE_LONG_PRESS: return "STATE_LONG_PRESS";
			case STATE_CHECK_HOLD: return "STATE_CHECK_HOLD";
		}
		return "";
	}
	
	// Subscribers for state updates:
	void callbackTargetSpeed(const std_msgs::Int16::ConstPtr& msg)
	{
		this->target_speed = msg->data;
		ROS_INFO("New target speed: %d", this->target_speed);
	}
	
	void callbackTargetDistanceSetting(const std_msgs::UInt8::ConstPtr& msg)
	{
		this->target_distance_setting = msg->data;
		ROS_INFO("New distance setting: %d", this->target_distance_setting);
	}
	
	void callbackSetSpeed(const std_msgs::Int16::ConstPtr& msg)
	{
		this->set_speed = msg->data;
	}
	
	void callbackSetDistance(const std_msgs::Int16::ConstPtr& msg)
	{
		this->set_distance = msg->data;
	}
	
	void callbackControlsAllowed(const std_msgs::Bool::ConstPtr& msg)
	{
		this->controls_allowed = msg->data;
		if(state == STATE_CONTROLS_OFF) {
			if(controls_allowed == true) {
				transtionToState(STATE_IDLE);
			}
		} else {
			if(controls_allowed == false) {
				transtionToState(STATE_CONTROLS_OFF);
			}
		}
		
	}
	
	
	
	
	// Helper functions:
	int magDif(){
		return abs(target_speed - set_speed);
	}
	
	void publishButtonRequest(NissanButton button) {
		std_msgs::UInt8 msg;
		msg.data = (unsigned char) button;
		publisherButtonRequest.publish(msg);
	}
	
	
	// State handling:
	void enterState(CommandState newState) {
		switch (newState) {
			case STATE_CONTROLS_OFF:
				publishButtonRequest(NISSAN_BUTTON_OFF);
				break;
				
				// The rest represent CONTROLS_ON
			case STATE_IDLE:
				break;
				
			case STATE_SHORT_PRESS:
				decimatorShortPress = 0;	// timer reset
				// figure out button
				if(distBad()) {
					buttonToSend = NISSAN_BUTTON_DISTANCE;
				} else if(set_speed > target_speed) {
					buttonToSend = NISSAN_BUTTON_SET;	// Down
				} else {
					buttonToSend = NISSAN_BUTTON_RES;	// up
				}
				publishButtonRequest(buttonToSend);
				break;
				
			case STATE_TURN_OFF:
				decimatorRelease = 0;	// timer reset
				publishButtonRequest(NISSAN_BUTTON_OFF);
				break;
				
			case STATE_LONG_PRESS:
				decimatorLongPress = 0;	// timer reset
				if(set_speed > target_speed) {
					buttonToSend = NISSAN_BUTTON_SET;	// Down
				} else {
					buttonToSend = NISSAN_BUTTON_RES;	// up
				}
				publishButtonRequest(buttonToSend);
				break;
				
			case STATE_CHECK_HOLD:
				break;
				
		}
		state = newState;
	}
	
	void exitState(CommandState state) {
		switch (state) {
			case STATE_CONTROLS_OFF:
				break;
				
				// The rest represent CONTROLS_ON
			case STATE_IDLE:
				break;
				
				
			case STATE_CHECK_HOLD:
				break;
				
			case STATE_SHORT_PRESS:
				break;
				
			case STATE_TURN_OFF:
				break;
				
			case STATE_LONG_PRESS:
				break;
				
				
		}
	}
	
	void transtionToState( CommandState newState ) {
		ROS_INFO("Transitiong from state %s to %s", stateToName(state), stateToName(newState));
		exitState(state);
		enterState(newState);
	}
	
public:
	
	SetPointToAccButtonLogic(ros::NodeHandle* nodeHandle) {
		state = STATE_CONTROLS_OFF;
		target_speed = 20;
		target_distance_setting = 3;
		set_speed = 20;
		set_distance = 3;
		controls_allowed = false;
		
		this->nodeHandle = nodeHandle;
		//		publisherButtonRequest = nodeHandle->advertise<std_msgs::UInt8>("/car/cruise/nissan_button_request", 1000);
		//
		//		subscriberCmdVel = nodeHandle->subscribe("/car/cruise/cmd_vel", 1000, &SetPointToAccButtonLogic::callbackCmdVel, this);
		//		subscriberDistanceSetting = nodeHandle->subscribe("/acc/distance_setting", 1000, &SetPointToAccButtonLogic::callbackDistanceSetting, this);
		//		subscriberSetSpeed = nodeHandle->subscribe("/acc/set_speed", 1000, &SetPointToAccButtonLogic::callbackSetSpeed, this);
		//
		publisherButtonRequest = nodeHandle->advertise<std_msgs::UInt8>("/car/cruise/cmd_btn", 1000);
		
		subscriberControlsAllowed = nodeHandle->subscribe("/car/libpanda/controls_allowed", 1000, &SetPointToAccButtonLogic::callbackControlsAllowed, this);
		subscriberTargetSpeed = nodeHandle->subscribe("/target_speed_setting", 1000, &SetPointToAccButtonLogic::callbackTargetSpeed, this);
		subscriberTargetDistanceSetting = nodeHandle->subscribe("/target_gap_setting", 1000, &SetPointToAccButtonLogic::callbackTargetDistanceSetting, this);
		subscriberSetSpeed = nodeHandle->subscribe("/acc/set_speed", 1000, &SetPointToAccButtonLogic::callbackSetSpeed, this);
		subscriberSetDistance = nodeHandle->subscribe("/acc/set_distance", 1000, &SetPointToAccButtonLogic::callbackSetDistance, this);
		
	}
	
	
	// state variables are updated by ROS subscribers here
	
	bool distBad() {
		return target_distance_setting != set_distance;
	}
	
	void process( ) {
		switch (state) {
			case STATE_CONTROLS_OFF:
				break;
				
				// The rest represent CONTROLS_ON
			case STATE_IDLE:
				if( distBad() ||
				   ((magDif() < 3) && (magDif() > 0)) ) {
					transtionToState(STATE_SHORT_PRESS);
				}
				
				if(!distBad()  &&
				   (magDif() >= 3)) {
					transtionToState(STATE_LONG_PRESS);
				}
				break;
				
				
				
			case STATE_SHORT_PRESS:
				// Send the correct corresponding button here
				if(decimatorShortPress++ > PROCESS_DECIMATOR_PRESS) {
					transtionToState(STATE_TURN_OFF);
				}
				break;
				
			case STATE_TURN_OFF:
				// Send NISSAN_BUTTON_OFF to the button
				if(decimatorRelease++ > PROCESS_DECIMATOR_RELEASE) {
					transtionToState(STATE_IDLE);
				}
				break;
				
			case STATE_LONG_PRESS:
				// Send the correct corresponding button here
				if(decimatorLongPress++ > PROCESS_DECIMATOR_LONG_PRESS) {
					transtionToState(STATE_CHECK_HOLD);
				}
				break;
				
				
			case STATE_CHECK_HOLD:
				// decide which path to take
				if(distBad() ||
				   (magDif() < 3)) {
					transtionToState(STATE_TURN_OFF);
					return;
				}
				if(!distBad() ||
				   (3 <= magDif())) {
					transtionToState(STATE_LONG_PRESS);
				}
				
				break;
				
		}
	}
};







int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "nissan_target_speed_to_acc_buttons");
	ROS_INFO("Initializing ..");
	
	ros::NodeHandle nh;
	
	//    ros::spin();
	
	SetPointToAccButtonLogic mSetPointToAccButtonLogic(&nh);
	//ros::Publisher publisherGpsActive = nh.advertise<std_msgs::Bool>("/car/panda/gps_active", 1000);
	
	ros::Rate mainLoopRate(PROCESS_RATE);
	while(ros::ok()) {
		// publish that we are active:
		//std_msgs::Bool msgGpsActive;
		//msgGpsActive.data = true;
		//publisherGpsActive.publish( msgGpsActive );
		
		mSetPointToAccButtonLogic.process();//1.0/PROCESS_RATE);
		// Normal ROS rate limiting:
		mainLoopRate.sleep();
		ros::spinOnce();
		
		
	}
	
	// Cleanup:
	
	return 0;
}
