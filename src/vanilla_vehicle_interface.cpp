/*
 Authors: Matt Bunting, Matt Nice
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
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
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
#include "sensor_msgs/NavSatFix.h"
#include "sensor_msgs/NavSatStatus.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
// Libpanda headers:
#include "panda/toyota.h"
#include <panda.h>
#include <sstream>

/*
 This ROS node interfaces libpanda's ToyotaHandler

 Publishers:
 1) /car/can/raw - std_msgs/String -  This publishes CAN data of interest where the can_to_ros node named subs_fs.cpp can interpret values

 */

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


		if (
      //this is where the msgs to be published start
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


			)
		{
			pub_.publish(msgs);
		}
	}

public:
	CanToRosPublisher() {
		pub_ = nh1.advertise<std_msgs::String>("/car/can/raw", 1000);

	}

};

class PublishGpsObserver : public Panda::GpsListener {
private:
	ros::NodeHandle nhPublishGps;
	ros::Publisher pub_fix;
	ros::Publisher pub_gpstime;
	void newDataNotification( Panda::GpsData* gpsData ) {
		time_t gpsTime_t = mktime(&gpsData->time);

		sensor_msgs::NavSatFix fix_position;
		sensor_msgs::TimeReference fix_time;
		ros::Time current_time = ros::Time::now();

		fix_position.header.stamp = current_time;
		fix_position.status.status = (gpsData->info.status == 'A' ? sensor_msgs::NavSatStatus::STATUS_FIX : sensor_msgs::NavSatStatus::STATUS_NO_FIX); //Marked as active if we get the active character
		fix_position.status.service = sensor_msgs::NavSatStatus::SERVICE_GPS; //Flagged as normal GPS
		fix_position.latitude = gpsData->pose.latitude;
		fix_position.longitude = gpsData->pose.longitude;
		fix_position.altitude = gpsData->pose.altitude;

		double hdop_squared_half_sqrt = sqrt(gpsData->quality.HDOP * gpsData->quality.HDOP / 2.0);
		double vdop = gpsData->quality.VDOP;
		double covariance_diagonal[] = {hdop_squared_half_sqrt, hdop_squared_half_sqrt, vdop};
		for (size_t i = 0; i < 3; i++) {
			for (size_t j = 0; j < 3; j++) {
				size_t final_index = (i * 3) + j;
				fix_position.position_covariance[final_index] = covariance_diagonal[i] * covariance_diagonal[j];
			}
		}

		fix_position.position_covariance_type = sensor_msgs::NavSatFix::COVARIANCE_TYPE_APPROXIMATED; // Approximated as per above.

		fix_time.time_ref = ros::Time((uint32_t)gpsTime_t, ((uint32_t)gpsData->timeMilliseconds) * 1000000);
		fix_time.header.stamp = current_time;
		fix_time.source = "Libpanda";

		pub_fix.publish(fix_position);
		pub_gpstime.publish(fix_time);
	}
public:
	PublishGpsObserver() {
		pub_fix = nhPublishGps.advertise<sensor_msgs::NavSatFix>("/gps_fix", 1000);
		pub_gpstime = nhPublishGps.advertise<sensor_msgs::TimeReference>("/gps_fix_time", 1000);
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
	ros::init(argc, argv, "vehicle_interface"); //should this be vehicle_interface_nissan
	ROS_INFO("Initializing ..");

	ros::NodeHandle nh;

	const char filenameGpsStatus[] = "/etc/libpanda.d/pandaHaveGPS";
	writeToFileThenClose(filenameGpsStatus, "-1\n");

	ROS_INFO("Starting CanToRosPublisher...");
	CanToRosPublisher canToRosPublisher;

  //panda handling structure:
	Panda::Handler pandaHandler;

	double epsilon = 0.2;	// If system time is off from GPS time by this amount, update time.
	Panda::SetSystemTimeObserver mSetSystemTimeObserver(epsilon);
	pandaHandler.addGpsObserver(mSetSystemTimeObserver);

	Panda::GpsTracker mGpsTracker;	// Saves to /etc/libpanda.d/latest_gps
	pandaHandler.addGpsObserver(mGpsTracker);

	PublishGpsObserver publishGpsTracker;
	pandaHandler.addGpsObserver(publishGpsTracker);

  // Initialize Libpanda with ROS publisher:
	pandaHandler.addCanObserver(canToRosPublisher);

  ROS_INFO("Initializing PandaHandler...");
	pandaHandler.initialize();

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

	system(commandToCreateFolder.c_str()); // Creating a directory

	pandaHandler.getCan().saveToCsvFile(canDataFilename.c_str());
	pandaHandler.getGps().saveToCsvFile(gpsDataFilename.c_str());

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
	pandaHandler.stop();
	writeToFileThenClose(filenameGpsStatus, "-1\n");

    return 0;
}
