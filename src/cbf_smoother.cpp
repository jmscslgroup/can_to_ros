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
 Notes from George Gunter:

 This is an acceleration -> acceleration supervisory controller with the basicm implementation of:
 a_cmd_safe = min(a_cmd, f(v,s,delta_v))

 Publishers:
 1) /cmd_accel_safe -- The output acceleration "a_cmd_safe"

 Subscribers:
 1) /cmd_accel -- The input acceleration "a_cmd"
 2) /vel -- The Ego vehicle speed "v"
 3) /lead_dist_869 -- The leader distance "s"
 4) /rel_vel_869 -- The Leader relative speed "delta_v"
 5)  -- The absolute acceleration of the leader vehicle "a_l" can be derived from v and delta_v

 Note from Bunting:
 We will need to infer a_l from /vel and /rel_vel_869 (v and delta_v, respectively).  However since cars cutting in and out cna change, or the leader vehicle can be lost, we need to consider if inference of a_l is valid.  First, let's get one more subscriber:

 6) /car/hud/mini_car_enable -- lets us know if the car's radar has a valid vehicle

 Secondly, we need to check for large jumps in lead_dist_869.  If the difference of two adjacent measurements have a magnitude greater than CUT_IN_DISTANCE, then a_l will be invalid.

 */

#include <sys/time.h> // for SimpleDerivative

// ROS headers:
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"

#define CUT_IN_DISTANCE (5.0)	// In meters

/*
 Functions ported from Python from George Gunter:
 def cbf_filter(v,s,delta_v,a_l):
	 tg_accel = time_gap_filter(v,s,delta_v)
	 ca_accel = collision_avoid_filter(s,delta_v,a_l)
	 return min([tg_accel,ca_accel])

 def time_gap_filter(v,s,delta_v):
	 k = 0.1
	 t_min = 2.0
	 a_max = (1/t_min)*delta_v + (k/t_min)*(s - t_min*v)
	 return a_max

 def collision_avoid_filter(s,delta_v,a_l):
   s_min = 15.0
	 k1 = 0.5
	 k2 = 0.5
	 a_max = a_l + (k1+k2)*(delta_v) + (k1*k2)*(s-s_min)
	 return a_max
 */
double cbf_filter( double v, double s, double delta_v, double a_l) {
	double tg_accel = time_gap_filter(v,s,delta_v);
	double ca_accel = collision_avoid_filter(s,delta_v,a_l);
	return min( tg_accel, ca_accel);
//	return tg_accel < ca_accel ? tg_accel : ca_accel;
}

double time_gap_filter( double v, double s, double delta_v) {
	double k = 0.1;
	double t_min = 2.0;
	double a_max = (1.0/t_min)*delta_v + (k/t_min)*(s - t_min*v);
	return a_max;
}

double collision_avoid_filter( double s, double delta_v, double a_l) {
	double s_min = 15.0; // Added by George Gunter, changes the minimum desired space-gap
	double k1 = 0.5;
	double k2 = 0.5;
	double a_max = a_l + (k1+k2)*(delta_v) + (k1*k2)*(s-s_min);
	return a_max;
}

 /*
  Computes a simple time-based derivative given an input
  */
class SimpleDerivative {
	double priorSample;
	struct timeval priorTime;

public:

	double compute( double input ) {
		double output;

		struct timeval currentTime;
		gettimeofday(&currentTime, NULL);

		double dTime = (double)(currentTime.tv_sec - priorTime.tv_sec) + ((double)(currentTime.tv_usec - priorTime.tv_usec))/1000000.0;

		output = (input - priorSample) / dTime;

		priorSample = input;
		priorTime = currentTime;

		return output;
	}

	SimpleDerivative() {
		priorSample = 0;

		gettimeofday(&priorTime, NULL);
	}
}

/*
 ROS CBF stitching
 */

class ControlBarrierFunctionSmoother {
private:

	// Information from ROS subscribers:
	bool leadVehicleVisible;
	double speed;	// of ego
	double speedPrior;
	double leaderRelativeSpeed;
	double leaderRelativeSpeedPrior;
	double leaderRelativeDistance;
	double leaderRelativeDistancePrior;

	// Inferred information:
	double leaderAcceleration;	// Absolute
	bool validLeaderAcceleration;
	int leaderAccelerationSampleCount;

	// Other information:
	int startSeconds;
	double timeSinceStart;
	double timeSinceStartPrior;

	bool cutInOrCutOutOccured;

	SimpleDerivative mSimpleDerivative;

	// Pubs and Subs:
	ros::Subscriber subscriberMiniCarEnable;
	ros::Subscriber subscriberLeadDistance;
	ros::Subscriber subscriberLeadRelativeSpeed;
	ros::Subscriber subscriberSpeed;
	ros::Subscriber subscriberCommandAcceleration;

	ros::Publisher publisherCommandAccelerationSafe;


public:

	void callbackMiniCarEnable(const std_msgs::Bool::ConstPtr& msg) {
		leadVehicleVisible = msg->data;

		if (!leadVehicleVisible) {
			validLeaderAcceleration = false;
		}
	}

	void callbackLeadDistance(const std_msgs::Float64::ConstPtr& msg) {
		leaderRelativeDistancePrior = leaderRelativeDistance;
		leaderRelativeDistance = msg->data;

		// Need to check for large changes for cut-ins, which invalidate acceleration inference
		if ( fabs(leaderRelativeDistance - leaderRelativeDistancePrior) > CUT_IN_DISTANCE ) {
			// Cut-in or cut-out occured
			cutInOrCutOutOccured = true;
			validLeaderAcceleration = false;
		} else {
			cutInOrCutOutOccured = false;
		}
	}

	void callbackLeadRelativeSpeed(const std_msgs::Float64::ConstPtr& msg) {
		leaderRelativeSpeedPrior = leaderRelativeSpeed;
		leaderRelativeSpeed = msg->data;

		// We also need to infer the acceleration;
		leaderAcceleration = mSimpleDerivative.compute( speed + leaderRelativeSpeed );

		// Wait for two function calls before stating valid acceleration:
		if ( !validLeaderAcceleration ) {
			leaderAccelerationSampleCount++;
			if (leaderAccelerationSampleCount >= 2) {	// need 2 good measurements for SimpleDerivative
				validLeaderAcceleration = true;
				leaderAccelerationSampleCount = 0;	// reset for next time
			}
		}
	}

	void callbackSpeed(const geometry_msgs::Twist::ConstPtr& msg) {
		speedPrior = speed;
		speed = msg->data.Linear.x;	// TODO check that Linear.x is the correct data for ego speed
	}


	void callbackCommandAcceleration(const std_msgs::Float64::ConstPtr& msg) {
		// Input:
		double cmd_accel_desired = msg->data;

		// Output:
		std_msgs::Float64 cmd_accel_safe;
		cmd_accel_safe.data = cmd_accel_desired;	// default value

		if ( leadVehicleVisible && !cutInOrCutOutOccured && validLeaderAcceleration) {
			// Perform CBF with current states:
			double v = speed;
			double delta_v = leaderRelativeSpeed;
			double s = leaderRelativeDistance;
			double a_l = leaderAcceleration;
			cmd_accel_safe.data = min(cmd_accel_desired, cbf_filter(v, s, delta_v, a_l));
		}

		// Publish the output:
		publisherCommandAccelerationSafe.publish( cmd_accel_safe );
	}

	Disengager(ros::NodeHandle* nodeHandle) {
		validLeaderAcceleration = false;
		leaderAccelerationSampleCount = 0;

		publisherCommandAccelerationSafe = nodeHandle->advertise<std_msgs::Float64>("/cmd_accel_safe", 1000);


		subscriberMiniCarEnable = nodeHandle->subscribe("/car/hud/mini_car_enable", 1000, &CruiseStartSafetyCheck::callbackMiniCarEnable, this);
		subscriberLeadDistance = nodeHandle->subscribe("/lead_dist_869", 1000, &Disengager::callbackLeadDistance, this);
		subscriberLeadRelativeSpeed = nodeHandle->subscribe("/rel_vel_869", 1000, &Disengager::callbackLeadRelativeSpeed, this);
		subscriberSpeed = nodeHandle->subscribe("/vel", 1000, &Disengager::callbackSpeed, this);
		subscriberCommandAcceleration = nodeHandle->subscribe("/cmd_accel", 1000, &Disengager::callbackCommandAcceleration, this);
	}

};



int main(int argc, char **argv) {
	ros::init(argc, argv, "cbf_smoother");
	ROS_INFO("CBF Smoother is running...");

	ros::NodeHandle nh;

	ControlBarrierFunctionSmoother mControlBarrierFunctionSmoother(&nh);

    ros::spin();

    return 0;
}
