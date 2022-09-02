/*
 Authors: Safwan Elmadani, Matt Bunting
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
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
// Libpanda headers:
#include <panda.h>
#include <sstream>
#include <fstream>

// A simple concrete instance of a CAN listener
class CanToRosPublisher : public Panda::CanListener {

private:
	ros::NodeHandle nh1;
	ros::Publisher pub_;
	std::stringstream ss;
	std::ofstream csvfile;


	void newDataNotification( Panda::CanFrame* canData ) {
	char messageString[200000];
	char messageTofile[200000];
		sprintf( messageString, "%d.%06d ", (unsigned int)0, (int)0);
		sprintf( messageString,"%s%d %d ", messageString, (int)canData->bus, canData->messageID);
		for (int i = 0; i < canData->dataLength; i++) {
			sprintf( messageString, "%s%02x", messageString, canData->data[i]);
		}
		sprintf( messageString, "%s %d", messageString, canData->dataLength);

		std_msgs::String msgs;
    	msgs.data = messageString;

		if (canData->messageID == 139 || canData->messageID == 303 || canData->messageID== 771
		    // || canData->messageID== 869
		  //  || canData->messageID == 381
		  //  || canData->messageID == 382
		  //  || canData->messageID == 385
		  //  || canData->messageID == 386
		  //  || canData->messageID == 389
		  //  || canData->messageID == 390
		  //  || canData->messageID == 393
		  //  || canData->messageID == 394
		  //  || canData->messageID == 398
		  //  || canData->messageID == 399
		  //  || canData->messageID == 405
		  //  || canData->messageID == 407
		  //  || canData->messageID == 411
		  //  || canData->messageID == 412
		  //  || canData->messageID == 415
		  //  || canData->messageID == 416
		  //  || canData->messageID == 419
		  //  || canData->messageID == 420
		    // || canData->messageID == 923
		    // || canData->messageID == 924
				// || canData->messageID == 936
		    // || canData->messageID == 951
				// || canData->messageID == 954
		    // || canData->messageID == 958
				|| canData->messageID == 1119
		    || canData->messageID == 1487
			)
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
// A simple concrete instance of a GPS listener
/*
class SimpleGpsObserver : public Panda::GpsListener {
private:
	int notificationCount = 0;
	void newDataNotification( Panda::GpsData* gpsData ) {
		notificationCount++;
		if(notificationCount > 10) {
			std::cerr << "g";
			notificationCount = 0;
		}
	}
};
*/
using namespace std;
int main(int argc, char **argv) {
	// Initialize ROS stuff:
	ros::init(argc, argv, "publish_libpanda");

	ROS_INFO("Got parameter : %s", argv[1]);

	// Initialize Libpanda with ROS publisher:
	CanToRosPublisher canToRosPublisher;
	//SimpleGpsObserver myGpsObserver;
	// Initialize Usb, this requires a connected Panda
	Panda::Handler pandaHandler;

	pandaHandler.addCanObserver(canToRosPublisher);
	//pandaHandler.addGpsObserver(myGpsObserver);
	// Let's roll
	pandaHandler.initialize();

	// Let ROS do its thing:
	ros::spin();

	// Cleanly close USB device
	pandaHandler.stop();
	return 0;
}
