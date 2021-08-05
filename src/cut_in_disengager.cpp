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
 This ROS disengages the cruise controller if a driver cuts in front of the vehicle and triggers a large deceleration request

 This disengages based on the following events:
  - A vehicle changes lanes into the car in from of the vehicles within the last 2 seconds
  - The controller commands a deceleration of less than a minimum
 
 Publishers:
 1) /cmd_accel_safe -- std_msgs/Float64 - The topic /cmd_accel is passed through to this topic if this node does not think a disengage is required
 2) /car/hud/cruise_cancel_request - std_msgs/Bool - This node will publish a true to this topic during a cut in, and a false after ready to allow re-engagements
 
 Subscribers:
 1) /cmd_accel -- std_msgs/Float64 - Sniffs the controller request to check for disengagements while passing through to /cmd_accel_safe
 2) /lead_dist_869 -- std_msgs/Float64 - From node subs_fs, looks for cut in events
 
 
 */

#include <deque>
#include <mogi/thread.h>	// just for mutual exclusion

// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"

#define TIME_TO_SAVE_LEAD_DISTANCE_DIFFS (2)	// in seconds
#define CMD_ACCEL_MIN_ACCEPTABLE (-2.5)
#define NUM_LEAD_DISTANCE_DIFFERENCES (5 * TIME_TO_SAVE_LEAD_DISTANCE_DIFFS)	// Assuming 5Hz response, this means 2 seconds
#define CUT_IN_DISTANCE_MINIMUM (-5.0)
#define CANCEL_REQUEST_DELAY_COUNTER (30 * (TIME_TO_SAVE_LEAD_DISTANCE_DIFFS + 1.0/5.0) + 1)	// This runs based on the cmd_accel subscribe rate, will release cancel requests after this many of recieved messages


class Disengager : Mogi::Thread {
private:
	std::deque<double> lastLeadDistanceDifferences;
	double lastLeadDistance;
	
	bool lastCancelRequest;
	int cancelRequestDelayCounter;
	
	ros::Subscriber subscriberLeadDistance;
	ros::Subscriber subscriberCommandAcceleration;
	
	ros::Publisher publisherCommandAccelerationSafe;
	ros::Publisher publisherCruiseCancelRequest;
	
	
public:
	
	bool cutInOccurred() {
		bool cutIn = false;
		lock();
		for (std::deque<double>::iterator it = lastLeadDistanceDifferences.begin();
			 it != lastLeadDistanceDifferences.end();
			 it++) {
			
			cutIn |= (*it) < CUT_IN_DISTANCE_MINIMUM;
			if (cutIn) {
				break;
			}
		}
		unlock();
		
		return cutIn;
	}
	
	void callbackLeadDistance(const std_msgs::Float64::ConstPtr& msg) {
		double currentLeadDistance = msg->data;
		
		if (lastLeadDistanceDifferences.size() == 0) {
			lastLeadDistance = currentLeadDistance;
			lastLeadDistanceDifferences.push_back( 0 );
			return;
		}
		
		lock();
		while(lastLeadDistanceDifferences.size() > NUM_LEAD_DISTANCE_DIFFERENCES ) {
			lastLeadDistanceDifferences.pop_front();
		}
		lastLeadDistanceDifferences.push_back( currentLeadDistance - lastLeadDistance );
		unlock();
		
		lastLeadDistance = currentLeadDistance;
	}

	
	void callbackCommandAcceleration(const std_msgs::Float64::ConstPtr& msg) {
		double cmd_accel_desired = msg->data;
		
		// Set default expected value:
		std_msgs::Float64 cmd_accel_safe;
		cmd_accel_safe.data = cmd_accel_desired;
		
		if (lastCancelRequest == true) {
			cmd_accel_safe.data = 0;	// still need to send 0
			
			if (cancelRequestDelayCounter++ >= CANCEL_REQUEST_DELAY_COUNTER) {
				cancelRequestDelayCounter = 0;
				
				ROS_INFO("Disengager is now allowing reengagements");
				
				// Release the cancel request:
				std_msgs::Bool msgCancelRequest;
				msgCancelRequest.data = false;
				publisherCruiseCancelRequest.publish(msgCancelRequest);
				lastCancelRequest = false;
			}
		}
		
		if ((cmd_accel_desired < CMD_ACCEL_MIN_ACCEPTABLE) &&
			cutInOccurred() &&
			(cancelRequestDelayCounter == 0)) {
			ROS_INFO("Disengager node is disengaging the cruise controller!");
			// Kill the acceleration command amount:
			cmd_accel_safe.data = 0;
			
			// Cancel the cruise controller:
			std_msgs::Bool msgCancelRequest;
			msgCancelRequest.data = true;
			publisherCruiseCancelRequest.publish(msgCancelRequest);
			lastCancelRequest = true;
			
			cancelRequestDelayCounter++;
		}
		
		publisherCommandAccelerationSafe.publish( cmd_accel_safe );
	}
	
	Disengager(ros::NodeHandle* nodeHandle) {
		lastCancelRequest = false;
		cancelRequestDelayCounter = 0;
		
		publisherCommandAccelerationSafe = nodeHandle->advertise<std_msgs::Float64>("/cmd_accel_safe", 1000);
		publisherCruiseCancelRequest = nodeHandle->advertise<std_msgs::Bool>("/car/hud/cruise_cancel_request", 1000);
		
		subscriberLeadDistance = nodeHandle->subscribe("/lead_dist_869", 1000, &Disengager::callbackLeadDistance, this);
		subscriberCommandAcceleration = nodeHandle->subscribe("/cmd_accel", 1000, &Disengager::callbackCommandAcceleration, this);
	}
	
};



int main(int argc, char **argv) {
	ros::init(argc, argv, "disengager");
	ROS_INFO("Disengager is running...");

	ros::NodeHandle nh;
	
	Disengager mDisengager(&nh);
	
    ros::spin();
	
    return 0;
}
