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
#include "panda/nissan.h"
#include "panda/controller.h"	// No more direct vehicle controller class instantiation
#include <panda.h>
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


class Control {
private:
	ros::NodeHandle* n_;
	ros::Subscriber subscribers[3];
//	ros::Subscriber sub_;
//	ros::Subscriber subscriberMiniCarHud;
//	ros::Subscriber subscriberCruiseCancel;
	// Initialize panda and toyota handlers
//	Panda::ToyotaHandler* toyotaHandler;
	Panda::Controller* pandaController;
	
	// These are dynamically populate based on VIN reading:
	Panda::ToyotaHandler* toyotaHandler;
	Panda::NissanAccButtonController* nissanAccButtonHandler;

public:
	
	/*
	 Toyota subscribers:
	 */
	void callbackAccelInput(const std_msgs::Float64::ConstPtr& msg)
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

	void callbackCruiseCancel(const std_msgs::Bool::ConstPtr& msg)
	{
		toyotaHandler->setHudCruiseCancelRequest(msg->data);
	}
	
	
	/*
	 Nisaan ACC button subscribers:
	 */
	void callbackAccButtonRequest(const std_msgs::UInt8::ConstPtr& msg)
	{
		ROS_INFO("vehicle_interface Received button request: %d: %s", (int)msg->data, nissanButtonToStr((Panda::NissanButton)msg->data));
		nissanAccButtonHandler->sendButton((Panda::NissanButton)msg->data);
		ROS_INFO("vehicle_interface Done pressing button:    %d: %s", (int)msg->data, nissanButtonToStr((Panda::NissanButton)msg->data));
	}

	
	
	//Control(Panda::ToyotaHandler* toyotaHandler, ros::NodeHandle* nodeHandle) {
	Control(Panda::Controller* pandaController, ros::NodeHandle* nodeHandle) {
		n_ = nodeHandle;
		
		nissanAccButtonHandler = NULL;
		toyotaHandler = NULL;

//		this->toyotaHandler = toyotaHandler;
		this->pandaController = pandaController;
		// intializing a subscriber
//		sub_ = n_->subscribe("/commands", 1000, &Control::callback, this);
		if (pandaController->getPandaHandler()->getVehicleManufacturer() == Panda::VEHICLE_MANUFACTURE_NISSAN) {
			nissanAccButtonHandler = static_cast<Panda::NissanAccButtonController*>(pandaController);
			subscribers[0] = n_->subscribe("/car/cruise/cmd_btn", 1000, &Control::callbackAccButtonRequest, this);
		} else {
			toyotaHandler = static_cast<Panda::ToyotaHandler*>(pandaController);
			subscribers[0] = n_->subscribe("/car/cruise/accel_input", 1000, &Control::callbackAccelInput, this);
			subscribers[1] = n_->subscribe("/car/hud/mini_car_enable", 1000, &Control::callbackMiniCar, this);
			subscribers[2] = n_->subscribe("/car/hud/cruise_cancel_request", 1000, &Control::callbackCruiseCancel, this);
		}
	}

	~Control(){

	}
};

//class PandaStatusPublisher : public Panda::ToyotaListener {
class PandaStatusPublisher : public Panda::ControllerListener {
	ros::NodeHandle* nodeHandle;
	
	ros::Publisher publisherLibpandaControlsEnabled;
	ros::Publisher* publisherLibpandaAccButtonWireGood;

	ros::Publisher publisherPandaControlsEnabled;
	ros::Publisher publisherPandaGasInterceptorDetected;

	// void doAction() {
	// 	usleep(100000);
	// 	// TODO: this should run at its own rate and not be dependent on CAN events:
	// 	std_msgs::Bool msgControlsEnabled;
	// 	std_msgs::Bool msgGasInterceptorDetected;
  //
	// 	msgControlsEnabled.data = false;
	// 	msgGasInterceptorDetected.data = false;
	// 	if(toyotaHandler != NULL) {
	// 		msgControlsEnabled.data = toyotaHandler->getControlsAllowed();
	// 		msgGasInterceptorDetected.data = toyotaHandler->getPandaHealth().gas_interceptor_detected;
	// 	}
	// 	publisherPandaControlsEnabled.publish( msgControlsEnabled );
	// 	publisherPandaGasInterceptorDetected.publish( msgGasInterceptorDetected );
	// }

//	void newControlNotification(Panda::ToyotaHandler* toyotaHandler) {
	void newControlNotification(Panda::Controller* controller) {
		std_msgs::Bool msgControlsEnabled;

//		msgControlsEnabled.data = controller->getPandaControlsAllowed();
		msgControlsEnabled.data = controller->getControlsAllowed();

		publisherLibpandaControlsEnabled.publish( msgControlsEnabled );
		
		if(controller->getPandaHandler()->getVehicleManufacturer() == Panda::VEHICLE_MANUFACTURE_NISSAN) {
			if(publisherLibpandaAccButtonWireGood == NULL) {
				publisherLibpandaAccButtonWireGood = new ros::Publisher;
				*publisherLibpandaAccButtonWireGood = nodeHandle->advertise<std_msgs::Bool>("/car/libpanda/acc_button_wire_connected", 1000);
			}
			
			Panda::NissanAccButtonController* nissanAccButtonHandler = static_cast<Panda::NissanAccButtonController*>(controller);
			
			std_msgs::Bool msgAccButtonWireConnected;
			msgAccButtonWireConnected.data = nissanAccButtonHandler->isHardwareConnectionGood();
			publisherLibpandaAccButtonWireGood->publish( msgAccButtonWireConnected );
		}
	}

	void newPandaHealthNotification(const PandaHealth& pandaHealth) {
		std_msgs::Bool msgControlsEnabled;
		std_msgs::Bool msgGasInterceptorDetected;

		msgControlsEnabled.data = pandaHealth.controls_allowed;
		msgGasInterceptorDetected.data = pandaHealth.gas_interceptor_detected;

		publisherPandaControlsEnabled.publish( msgControlsEnabled );
		publisherPandaGasInterceptorDetected.publish( msgGasInterceptorDetected );
	}

public:
	PandaStatusPublisher(ros::NodeHandle* nodeHandle) {
		this->nodeHandle = nodeHandle;
		
		publisherLibpandaControlsEnabled = nodeHandle->advertise<std_msgs::Bool>("/car/libpanda/controls_allowed", 1000);
		publisherLibpandaAccButtonWireGood = NULL;

		publisherPandaControlsEnabled = nodeHandle->advertise<std_msgs::Bool>("/car/panda/controls_allowed", 1000);
		publisherPandaGasInterceptorDetected = nodeHandle->advertise<std_msgs::Bool>("/car/panda/gas_interceptor_detected", 1000);
	}
	
	~PandaStatusPublisher() {
		if(publisherLibpandaAccButtonWireGood) {
			delete publisherLibpandaAccButtonWireGood;
		}
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

//	Panda::ToyotaHandler* toyotaHandler;
	Panda::Controller* pandaController;
	
	
	char messageString[1000];
//	char messageTofile[1000];
	
	void publishCanMessage( Panda::CanFrame* canData ) {
		//			sprintf( messageString, "%d.%06d ", (unsigned int)0, (int)0);
		//			sprintf( messageString,"%s%d %d ", messageString, (int)canData->bus, canData->messageID);
		sprintf( messageString, "0.000000 %d %d ", (int)canData->bus, canData->messageID);
		for (int i = 0; i < canData->dataLength; i++) {
			sprintf( messageString, "%s%02x", messageString, canData->data[i]);
		}
		sprintf( messageString, "%s %d", messageString, canData->dataLength);
		
		std_msgs::String msgs;
		msgs.data = messageString;
		
		pub_.publish(msgs);
	}

	void newDataNotification( Panda::CanFrame* canData ) {
		
		if( pandaController->getPandaHandler()->getVehicleManufacturer() == Panda::VEHICLE_MANUFACTURE_TOYOTA) {
			if ( // Toyota:
				
				//this is where the msgs to be published start
//				canData->messageID == 139 || canData->messageID == 37 || canData->messageID== 1570
//				|| canData->messageID== 869 || (canData->messageID>= 384 && canData->messageID<=399 )
//				|| canData->messageID== 552 || canData->messageID== 921 || canData->messageID== 467
				(canData->messageID==37&&canData->dataLength==8)
				||(canData->messageID==180&&canData->dataLength==8)
				||(canData->messageID==869&&canData->dataLength==7)
				||(canData->messageID==384&&canData->dataLength==8)
				||(canData->messageID==385&&canData->dataLength==8)
				||(canData->messageID==386&&canData->dataLength==8)
				||(canData->messageID==387&&canData->dataLength==8)
				||(canData->messageID==388&&canData->dataLength==8)
				||(canData->messageID==389&&canData->dataLength==8)
				||(canData->messageID==390&&canData->dataLength==8)
				||(canData->messageID==391&&canData->dataLength==8)
				||(canData->messageID==392&&canData->dataLength==8)
				||(canData->messageID==393&&canData->dataLength==8)
				||(canData->messageID==394&&canData->dataLength==8)
				||(canData->messageID==395&&canData->dataLength==8)
				||(canData->messageID==396&&canData->dataLength==8)
				||(canData->messageID==397&&canData->dataLength==8)
				||(canData->messageID==398&&canData->dataLength==8)
				||(canData->messageID==399&&canData->dataLength==8)
				||(canData->messageID==1570&&canData->dataLength==8)
				||(canData->messageID==467&&canData->dataLength==8)
				||(canData->messageID==835&&canData->dataLength==8)
				||(canData->messageID==921&&canData->dataLength==8)
				||(canData->messageID==552&&canData->dataLength==4)
				//this is where the msgs to be published end
				
				) {
					publishCanMessage(canData);
				}
		} else {
			if ( // Nissan:
				
				//this is where the msgs to be published start
				(canData->messageID==139&&canData->dataLength==48)
				||(canData->messageID==140&&canData->dataLength==48)
				||(canData->messageID==303&&canData->dataLength==12)
				||(canData->messageID==308&&canData->dataLength==64)
				||(canData->messageID==1119&&canData->dataLength==20)
				||(canData->messageID==1487&&canData->dataLength==48)
				//this is where the msgs to be published end
				
				) {
					publishCanMessage(canData);
				}
			
		}
		
	}

public:
//	CanToRosPublisher(ros::NodeHandle* nodeHandle, Panda::ToyotaHandler* handler) {
	CanToRosPublisher(ros::NodeHandle* nodeHandle, Panda::Controller* controller) {
//		toyotaHandler = handler;
		pandaController = controller;
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

//// This is a quick hacky function to allow for notifications of system time being set:
void writeToFileThenClose(const char* filename, const char* data) {
	FILE* file = fopen( filename, "w+");
	fwrite( data, 1, strlen(data), file);
	fclose(file);
};

int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "vehicle_interface");
	ROS_INFO("Initializing vehicle_interface...");

	ros::NodeHandle nh;

	const char filenameGpsStatus[] = "/etc/libpanda.d/pandaHaveGPS";
	writeToFileThenClose(filenameGpsStatus, "-1\n");


	// toyota controller structure:
	Panda::Handler pandaHandler;

	double epsilon = 0.2;	// If system time is off from GPS time by this amount, update time.
	Panda::SetSystemTimeObserver mSetSystemTimeObserver(epsilon);
	pandaHandler.addGpsObserver(mSetSystemTimeObserver);

	Panda::GpsTracker mGpsTracker;	// Saves to /etc/libpanda.d/latest_gps
	pandaHandler.addGpsObserver(mGpsTracker);

//	Panda::ToyotaHandler toyotaHandler(&pandaHandler);
	

	ROS_INFO("Initializing PandaHandler...");
	pandaHandler.initialize();
	
	ROS_INFO("Initializing pandaController from factory...");
	Panda::ControllerClient* pandaController = new Panda::ControllerClient(pandaHandler);
	
	
	
	if(pandaController->getController() == NULL) {
		ROS_ERROR("No VIN discovered, unable to build controller handler");
		
		// Delete the controller and kill the process, this cannot continue
//		delete pandaController;
//		exit(EXIT_FAILURE);
		
		// We can for set the VIN to continue with the follwing code, but best to just fail everything since somethin aint right:
		ROS_ERROR("Force setting the VIN to 5N1000000P0000000");
		pandaHandler.forceSetVin((const unsigned char*)"5N1000000P0000000");	// Hard coded VIN setting
		pandaController = new Panda::ControllerClient(pandaHandler);
		if(pandaController->getController() == NULL) {
			std::cerr << "ERROR 2: No VIN discovered, unable to make test controller" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

    // Initialize Libpanda with ROS publisher:
	ROS_INFO("Starting CanToRosPublisher...");
//	CanToRosPublisher canToRosPublisher(&nh, &toyotaHandler);
	CanToRosPublisher canToRosPublisher(&nh, pandaController->getController());


	ROS_INFO("Connecting  PandaStatusPublisher...");
	PandaStatusPublisher mPandaStatusPublisher(&nh);
//	toyotaHandler.addObserver(&mPandaStatusPublisher);
	pandaController->getController()->addObserver(&mPandaStatusPublisher);
	//SimpleGpsObserver myGpsObserver;
	// Initialize Usb, this requires a connected Panda
	//Panda::Handler pandaHandler;

	pandaHandler.addCanObserver(canToRosPublisher);

	// Initialize panda and toyota handlers
//	ROS_INFO("Starting ToyotaHandler...");
	//toyotaHandler.start();


	ROS_INFO("Starting Panda::Controller...");
//	Control vehicleControl(&toyotaHandler, &nh);
	Control vehicleControl(pandaController->getController(), &nh);
	pandaController->getController()->start();

	writeToFileThenClose(filenameGpsStatus, "0\n");	// state 0: on but time not set



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

	if(mSetSystemTimeObserver.hasTimeBeenSet()) {
		writeToFileThenClose(filenameGpsStatus, "1\n");	// GPS time sync done
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

//    ros::spin();

	ros::Publisher publisherGpsActive = nh.advertise<std_msgs::Bool>("/car/panda/gps_active", 1000);
	ros::Rate mainLoopRate(1); // 1Hz
	while(ros::ok()) {
		// publish that we are active:
		std_msgs::Bool msgGpsActive;
		msgGpsActive.data = true;
		publisherGpsActive.publish( msgGpsActive );

		// Normal ROS rate limiting:
		mainLoopRate.sleep();
		ros::spinOnce();
	}

	// Cleanup:
//	mPandaStatusPublisher.stop();
//	toyotaHandler.stop();
	pandaController->getController()->stop();
	pandaHandler.stop();
	writeToFileThenClose(filenameGpsStatus, "-1\n");

    return 0;
}
