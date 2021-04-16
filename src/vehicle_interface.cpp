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
		toyotaHandler->setSteerTorque(0.0);  // doesnt work yet
	}
	Control(Panda::ToyotaHandler* toyotaHandler){
		
		this->toyotaHandler = toyotaHandler;
		// intializing a subscriber
		sub_ = n_.subscribe("/commands", 1000, &Control::callback, this);
	}
	
	~Control(){
		
	}
};

class CanToRosPublisher : public Panda::CanListener {

private:
	ros::NodeHandle nh1;
	ros::Publisher pub_;
	std::stringstream ss;
	std::ofstream csvfile;
	
	
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
		    || canData->messageID== 869 || (canData->messageID>= 384 && canData->messageID<=399 ) || canData->messageID== 552 )
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

	}
	
public:
	CanToRosPublisher() {
		// std::time_t t=time(0);
		// struct tm * now = localtime( &t );
		// char buffer [256];
		// strftime (buffer,80,"%Y-%m-%d-%X.csv",now);
		// std::string filename=buffer;
		// std::replace(filename.begin(), filename.end(), ':', '-'); 
        // cout << filename << std::endl;
		pub_ = nh1.advertise<std_msgs::String>("/realtime_raw_data", 1000);

		//FIXME: use libpanda to create CAN and GPS files
		// csvfile.open(filename); 
		// csvfile <<"Time" << ","<< "Bus" << "," << "MessageID" << "," << "Message" << ","<< "MessageLength" << std::endl;

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
	
    // Initialize Libpanda with ROS publisher:
	CanToRosPublisher canToRosPublisher;
	//SimpleGpsObserver myGpsObserver;
	// Initialize Usb, this requires a connected Panda
	//Panda::Handler pandaHandler;
	
	pandaHandler.addCanObserver(canToRosPublisher);

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
