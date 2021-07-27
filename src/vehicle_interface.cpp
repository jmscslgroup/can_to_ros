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
#include "std_msgs/Bool.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
// Libpanda headers:
#include "panda/toyota.h"
#include <panda.h>
#include <sstream>

/*
 This ROS node interprets a libpanda ToyotaHandler to interface ROS
 
 Details:
 1) This publishes CAN dat of interest of a std_msgs/String type to /realtime_raw_data, where the can_to_ros node named subs_fs.cpp can interpret values
 
 2) This node subscribes for acceleration commands with a std_msgs/Float64 named /commands
 
 3) This node subscribes to /car/hud/mini_car_enable as type std_msgs/Bool to display a mini-vehicle on the car's HUD
 
 4) This node publishes two std_msgs/Bool topics named /car/panda/controls_allowed and /car/panda/gas_interceptor_detected for relaying states reported by the Panda device
 
 */


class Control {
private:
	ros::NodeHandle* n_;
	ros::Subscriber sub_;
	ros::Subscriber subscriberMiniCarHud;
	// Initialize panda and toyota handlers
	Panda::ToyotaHandler* toyotaHandler;
	
public:
	void callback(const std_msgs::Float64::ConstPtr& msg)
	{
		// use these functions to set the acceleration and steeting Tourque
		toyotaHandler->setAcceleration(msg->data);
		toyotaHandler->setSteerTorque(0.0);  // doesnt work yet
	}
	
	void callbackMiniCar(const std_msgs::Bool::ConstPtr& msg)
	{
		toyotaHandler->setHudMiniCar(msg->data);
//		ROS_INFO("Recieved mini-car data: %d\n", msg->data);
	}
	
	Control(Panda::ToyotaHandler* toyotaHandler, ros::NodeHandle* nodeHandle) {
		n_ = nodeHandle;
		
		this->toyotaHandler = toyotaHandler;
		// intializing a subscriber
		sub_ = n_->subscribe("/commands", 1000, &Control::callback, this);
		subscriberMiniCarHud = n_->subscribe("/car/hud/mini_car_enable", 1000, &Control::callbackMiniCar, this);
	}
	
	~Control(){
		
	}
};

class PandaStatusPublisher : public Mogi::Thread {
	Panda::ToyotaHandler* toyotaHandler;
	ros::Publisher publisherPandaControlsEnabled;
	ros::Publisher publisherPandaGasInterceptorDetected;
	
	void doAction() {
		usleep(100000);
		// TODO: this should run at its own rate and not be dependent on CAN events:
		std_msgs::Bool msgControlsEnabled;
		std_msgs::Bool msgGasInterceptorDetected;
		
		msgControlsEnabled.data = false;
		msgGasInterceptorDetected.data = false;
		if(toyotaHandler != NULL) {
			msgControlsEnabled.data = toyotaHandler->getControlsAllowed();
			msgGasInterceptorDetected.data = toyotaHandler->getPandaHealth().gas_interceptor_detected;
		}
		publisherPandaControlsEnabled.publish( msgControlsEnabled );
		publisherPandaGasInterceptorDetected.publish( msgGasInterceptorDetected );
	}
	
public:
	PandaStatusPublisher(ros::NodeHandle* nodeHandle, Panda::ToyotaHandler* handler) {
		this->toyotaHandler = handler;
		publisherPandaControlsEnabled = nodeHandle->advertise<std_msgs::Bool>("/car/panda/controls_allowed", 1000);
		publisherPandaGasInterceptorDetected = nodeHandle->advertise<std_msgs::Bool>("/car/panda/gas_interceptor_detected", 1000);
	}
	
};

class CanToRosPublisher : public Panda::CanListener {

private:
	ros::NodeHandle* nh1;
	ros::Publisher pub_;
//	ros::Publisher publisherCarSetSpeed;
//	ros::Publisher publisherPandaControlsEnabled;
	std::stringstream ss;
	std::ofstream csvfile;
	
	Panda::ToyotaHandler* toyotaHandler;
	
	void newDataNotification( Panda::CanFrame* canData ) {
	char messageString[200];
	char messageTofile[200];
		sprintf( messageString, "%d.%06d ", (unsigned int)0, (int)0);
		sprintf( messageString,"%s%d %d ", messageString, (int)canData->bus, canData->messageID);
		for (int i = 0; i < canData->dataLength; i++) {
			sprintf( messageString, "%s%02x", messageString, canData->data[i]);
		}
		sprintf( messageString, "%s %d", messageString, canData->dataLength);
		
		std_msgs::String msgs;
    	msgs.data = messageString;
		
		if (canData->messageID == 180 || canData->messageID == 37 || canData->messageID== 1570 
		    || canData->messageID== 869 || (canData->messageID>= 384 && canData->messageID<=399 ) 
			|| canData->messageID== 552 || canData->messageID== 921 || canData->messageID== 467 )
		{
			pub_.publish(msgs);
		}
		// sprintf( messageTofile, "%f,", ros::Time::now().toSec());
		// sprintf( messageTofile,"%s%d,%d,", messageTofile, (int)canData->bus, canData->messageID);
		// for (int i = 0; i < canData->dataLength; i++) {
		// 	sprintf( messageTofile, "%s%02x", messageTofile, canData->data[i]);
		// }
		// sprintf( messageTofile, "%s,%d", messageTofile, canData->dataLength);

		// csvfile << messageTofile << std::endl;

//		if ( canData->messageID == 869 ) {
//			if( toyotaHandler != NULL ) {
//				int valueOfLeadDistance = ((*(unsigned long*)canData->data) >> (39+1-8)) & 0xFF;
//				//			ROS_INFO("valueOfLeadDistance = %d\n", valueOfLeadDistance);
//				if( valueOfLeadDistance < 252 ) {
//					toyotaHandler->setHudMiniCar( true );
//				} else {
//					toyotaHandler->setHudMiniCar( false );
//				}
//			}
//		}
		
//		if ( canData->messageID == 921 ) {
//			std_msgs::UInt8 msgCruiseSetSpeed;
//			msgCruiseSetSpeed.data = ((*(unsigned long*)canData->data) >> (31+1-8)) & 0xFF;
//			publisherCarSetSpeed.publish( msgCruiseSetSpeed );
//		}
		
	}
	
public:
	CanToRosPublisher(ros::NodeHandle* nodeHandle, Panda::ToyotaHandler* handler) {
		toyotaHandler = handler;
		nh1 = nodeHandle;
		// std::time_t t=time(0);
		// struct tm * now = localtime( &t );
		// char buffer [256];
		// strftime (buffer,80,"%Y-%m-%d-%X.csv",now);
		// std::string filename=buffer;
		// std::replace(filename.begin(), filename.end(), ':', '-'); 
        // cout << filename << std::endl;
		pub_ = nh1->advertise<std_msgs::String>("/realtime_raw_data", 1000);
//		publisherCarSetSpeed = nh1->advertise<std_msgs::UInt8>("/car/cruise/ui_set_speed", 1000);
//		publisherPandaControlsEnabled = nh1->advertise<std_msgs::String>("/panda/controls_enabled", 1000);

		//FIXME: use libpanda to create CAN and GPS files
		// csvfile.open(filename); 
		// csvfile <<"Time" << ","<< "Bus" << "," << "MessageID" << "," << "Message" << ","<< "MessageLength" << std::endl;

	}
	
};


int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "send_commands");
	ROS_INFO("Initializing ..");

	ros::NodeHandle nh;
	
	// toyota controller structure:
	Panda::Handler pandaHandler;
	
	double epsilon = 0.2;	// If system time is off from GPS time by this amount, update time.
	Panda::SetSystemTimeObserver mSetSystemTimeObserver(epsilon);
	pandaHandler.addGpsObserver(mSetSystemTimeObserver);
	
	Panda::ToyotaHandler toyotaHandler(&pandaHandler);
	
	pandaHandler.initialize();
	

    // Initialize Libpanda with ROS publisher:
	CanToRosPublisher canToRosPublisher(&nh, &toyotaHandler);
	
	PandaStatusPublisher mPandaStatusPublisher(&nh, &toyotaHandler);
	//SimpleGpsObserver myGpsObserver;
	// Initialize Usb, this requires a connected Panda
	//Panda::Handler pandaHandler;
	
	pandaHandler.addCanObserver(canToRosPublisher);

	// Initialize panda and toyota handlers
	toyotaHandler.start();
	mPandaStatusPublisher.start();

	Control vehicleControl(&toyotaHandler, &nh);
	
	
	
	
	//  Set the sytem time here:
	ROS_INFO("Waiting to acquire satellites to set system time...");
//	ROS_INFO(" - Each \'.\' represents 100 NMEA messages received:");
	int lastNmeaMessageCount = 0;
	while ( !mSetSystemTimeObserver.hasTimeBeenSet() &&
		   ros::ok() ) {
		if (pandaHandler.getGps().getData().successfulParseCount-lastNmeaMessageCount > 500) {
			lastNmeaMessageCount = pandaHandler.getGps().getData().successfulParseCount;
			ROS_INFO(" - Recieved %d NMEA strings from GPS", lastNmeaMessageCount);
		}
		ros::spinOnce();
		usleep(10000);
	}
	
	
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
	
	pandaHandler.getCan().saveToCsvFile(canDataFilename.c_str());
	pandaHandler.getGps().saveToCsvFile(gpsDataFilename.c_str());
	
    ros::spin();
	
	// Cleanup:
	mPandaStatusPublisher.stop();
	toyotaHandler.stop();
	pandaHandler.stop();
    return 0;
}
