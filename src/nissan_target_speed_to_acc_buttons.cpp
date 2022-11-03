/*
 Author: Matt Bunting, Matt Nice
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
 This ROS node interfaces desired target ACC set points into button presses for vehicle_interface.cpp when operating a Nissan.  This node works with both target speed and target leader distance time-gap settings
 
 Publishers:
 1) /car/cruise/cmd_btn - std_msgs/UInt8 -  A request for vehicle_interface.cpp to perform an ACC button press via libpanda and its matthat.  See enum NissanButton for values

 
 Subscribers:
 1) /car/libpanda/controls_allowed - std_msgs/Bool - Determines whether button requests are accepted.  Affects state machine state
 2) /target_speed_setting - std_msgs/Int16 - The desired ACC speed set point (valid values 20 to ?) Units: mph
 3) /target_gap_setting - std_msgs/UInt8 - The desired ACC gap set point (valid values 1, 2, or 3) Units: # of bars
 4) /acc/set_speed - std_msgs/Int16 - The ACC's current speed set point, as reported by the car. Units: mph
 5) /acc/set_distance - std_msgs/Int16 - The ACC's current gap set point, as reported by the car.  Units: # of bars
 
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
	STATE_TURN_OFF,	// Button Release
	STATE_LONG_PRESS,
	STATE_CHECK_HOLD,
	//ACC_STATE_COME_HOME	// ON
} CommandState;


#define PROCESS_RATE (20.0)	// Hz

#define BUTTON_PRESS_TIME (0.25)	// seconds. This represents the total time the button is held
#define BUTTON_LONG_PRESS_TIME (0.1)	// seconds.  This does NOT represent held time, only the interval to check if it should still be held
#define BUTTON_RELEASE_TIME (0.25)	// seconds.  This represents a cooldown, the time between a button release and another button press

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
	// state variables are updated by ROS subscribers here
	void callbackTargetSpeed(const std_msgs::Int16::ConstPtr& msg)
	{
		this->target_speed = msg->data;
		//ROS_INFO("New target speed: %d", this->target_speed);
	}
	
	void callbackTargetDistanceSetting(const std_msgs::UInt8::ConstPtr& msg)
	{
		this->target_distance_setting = msg->data;
		//ROS_INFO("New distance setting: %d", this->target_distance_setting);
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
		//		if(state == STATE_CONTROLS_OFF) {
		//			if(controls_allowed == true) {
		//				transtionToState(STATE_IDLE);
		//			}
		//		} else {
		//			if(controls_allowed == false) {
		//				transtionToState(STATE_CONTROLS_OFF);
		//			}
		//		}
		
	}
	
	// Publisher function:
	void publishButtonRequest(NissanButton button) {
		std_msgs::UInt8 msg;
		msg.data = (unsigned char) button;
		publisherButtonRequest.publish(msg);
	}
	
	
	
	// Helper functions:
	int magDif(){
		return abs(target_speed - set_speed);
	}
	
	bool distBad() {
		return target_distance_setting != set_distance;
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
				if(distBad()) {		// Highest priority is for distance setting
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
		
		publisherButtonRequest = nodeHandle->advertise<std_msgs::UInt8>("/car/cruise/cmd_btn", 1000);
		
		subscriberControlsAllowed = nodeHandle->subscribe("/car/libpanda/controls_allowed", 1000, &SetPointToAccButtonLogic::callbackControlsAllowed, this);
		subscriberTargetSpeed = nodeHandle->subscribe("/target_speed_setting", 1000, &SetPointToAccButtonLogic::callbackTargetSpeed, this);
		subscriberTargetDistanceSetting = nodeHandle->subscribe("/target_gap_setting", 1000, &SetPointToAccButtonLogic::callbackTargetDistanceSetting, this);
		subscriberSetSpeed = nodeHandle->subscribe("/acc/set_speed", 1000, &SetPointToAccButtonLogic::callbackSetSpeed, this);
		subscriberSetDistance = nodeHandle->subscribe("/acc/set_distance", 1000, &SetPointToAccButtonLogic::callbackSetDistance, this);
		
		ROS_INFO("SetPointToAccButtonLogic has started with state %s, target_speed_setting = %d, target_gap_setting = %d", stateToName(state), target_speed, target_distance_setting);
	}
	
	
	void process( ) {
		// This is a check that must be performed in ANY state other than CONTROLS_OFF
		if((state != STATE_CONTROLS_OFF) &&
		   (controls_allowed == false)) {
			transtionToState(STATE_CONTROLS_OFF);
			return;
		}
		
		// The following is only valid under (state != STATE_CONTROLS_OFF)
		switch (state) {
			case STATE_CONTROLS_OFF:
				if(controls_allowed == true) {
					transtionToState(STATE_IDLE);
				}
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
	ROS_INFO("Initializing...");
	
	ros::NodeHandle nh;
	
	SetPointToAccButtonLogic mSetPointToAccButtonLogic(&nh);
	//ros::Publisher publisherGpsActive = nh.advertise<std_msgs::Bool>("/car/panda/gps_active", 1000);
	
	ros::Rate mainLoopRate(PROCESS_RATE);
	while(ros::ok()) {
		mSetPointToAccButtonLogic.process();
		
		// Normal ROS rate limiting:
		mainLoopRate.sleep();
		ros::spinOnce();
	}
	
	// Cleanup:
	
	return 0;
}
