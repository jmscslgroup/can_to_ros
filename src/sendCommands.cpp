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
// Libpanda headers:
#include "panda/toyota.h"
class Control {
private:
	ros::NodeHandle n_;
	ros::Subscriber sub_;
	// Initialize panda and toyota handlers
	Panda::ToyotaHandler* toyotaHandler;

public:
	void callback(const std_msgs::Float64::ConstPtr& msg)
	{
		// use these functions to set the acceleration and steeting Tourque
		toyotaHandler->setAcceleration(msg->data);
		toyotaHandler->setSteerTorque(msg->data);  // doesnt work yet
	}
	Control(Panda::ToyotaHandler* toyotaHandler){

		this->toyotaHandler = toyotaHandler;
		// intializing a subscriber
//		sub_ = n_.subscribe("/commands", 1000, &Control::callback, this);
		sub_ = n_.subscribe("/car/cruise/accel_input", 1000, &Control::callback, this);
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

	~Control(){

	}
};
int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "send_commands");
	ROS_INFO("Initializing ..");

  	// creating file names
	std::time_t t=time(0);
	struct tm * now = localtime( &t );
	char buffer1 [256];
  	char buffer2 [256];
	strftime (buffer1,80,"%Y-%m-%d-%X",now);
  	std::string bufferStr=buffer1;

  	std::replace(bufferStr.begin(), bufferStr.end(), ':', '-');

	strftime (buffer2,80,"%Y-%m-%d",now);
	std::string folderName = buffer2;
	std::replace(folderName.begin(), folderName.end(), '-', '_');
	std::ifstream file("/etc/libpanda.d/vin");
	std::string vin;
	std::getline(file, vin);
	std::string relativePath= "/var/panda/CyverseData/JmscslgroupData/PandaData";
	std::string commandToCreateFolder = "mkdir -p " + relativePath + "/" + folderName;


	std::string canDataFilename = relativePath + "/" + folderName + "/" + bufferStr + "_" + vin + "_CAN_Messages.csv";
	std::string gpsDataFilename = relativePath + "/" + folderName + "/" + bufferStr + "_" + vin + "_GPS_Messages.csv";

	// std::cout << commandToCreateFolder << std::endl;
	// std::cout << canDataFilename << std::endl;
	// std::cout << gpsDataFilename << std::endl;

  	system(commandToCreateFolder.c_str()); // Creating a directory




	// toyota controller structure:
	Panda::Handler pandaHandler;
	Panda::ToyotaHandler toyotaHandler(&pandaHandler);

	// Initialize panda and toyota handlers
	pandaHandler.initialize();
	toyotaHandler.start();
	pandaHandler.getCan().saveToCsvFile(canDataFilename.c_str());
    pandaHandler.getGps().saveToCsvFile(gpsDataFilename.c_str());

	Control vehicleControl(&toyotaHandler);

    ros::spin();
	// Cleanup:
	toyotaHandler.stop();
	pandaHandler.stop();
    return 0;
}
