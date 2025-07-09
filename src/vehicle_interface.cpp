/*
 Author: Matt Bunting, Matt Nice, Jonathan Sprinkle
 Copyright (c) 2020-2021 Arizona Board of Regents, 2022 Vanderbilt University
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
#include "panda/nissan.h"
#include "panda/controller.h"	// No more direct vehicle controller class instantiation
#include <panda.h>
#include <sstream>

/*
 This ROS node interfaces libpanda's ToyotaHandler

 Publishers:
 1) /car/can/raw - std_msgs/String -  This publishes CAN data of interest where the can_to_ros node named subs_fs.cpp can interpret values
 2) /car/panda/gas_interceptor_detected - std_msgs/Bool - Reported by the Panda is their gas interceptor hardware is detected
 3) /car/panda/controls_allowed - std_msgs/Bool -  Reported by the comma.ai panda.  This is not event-based from the Panda, but is regularly checked at 2 Hz to reset the Panda's heartbeat
 4)	/car/libpanda/controls_allowed - std_msgs/Bool -  Reported by logic within libpanda.  This is event based from libpanda using CAN messages.  When no events occur, this regularly published at 1 Hz which can be used to assess libpanda's control command health

 Nissan Publisher:
 6) /car/libpanda/acc_button_wire_connected - std_msgs/Bool - Informs the integrity of the electrical connections of the vehicle, from the ACC button wire throught the ADAS CAN bus
 7) /car/libpanda/busy_sending_button_press - std_msgs/Bool - Can inform another service that button presses are currently being processed.

 Subscribers:
 Toyota Subscribers:
 1) /car/cruise/accel_input - std_msgs/Float64 - This is for acceleration commands to be sent to the car's cruise controller (priorly known as /commands)
 2) /car/hud/mini_car_enable - std_msgs/Bool - When true, this will display a mini-vehicle on the car's HUD which cruise control is on and engaged
 3) /car/hud/cruise_cancel_request - std_msgs/Bool - When true  published, the cruise controller will disengage and notify the driver with an audible chime
 4) /car/steer/tq - std_msgs/FLoat64 - This is for steering torque commands to be sent to the vehicle.

 Nissan Subscribers:
 1) /car/cruise/cmd_btn - std_msgs/UInt8 - A button command to be sent to Nissan's ACC.  See libpanda's enum NissanButton for values
 2) /car/libpanda/control_request - std_msgs/Bool - Send 1 to request controls.  If the Nissan's ACC is on and engaged, /car/libpanda/controls_allowed will become true.  Set to 0 to relinquish control, disarming the mattHat's relay

 */


class Control {
private:
    ros::NodeHandle* n_;
    ros::Subscriber subscribers[4];
    ros::Publisher publisherBusySendingButtonPress;

    std_msgs::Bool msgBusyButton;
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
        //toyotaHandler->setSteerTorque(0.0);  // doesnt work yet
    }
    void callbackSteerTq(const std_msgs::Float64::ConstPtr& msg)
    {
        toyotaHandler->setSteerTorque(msg->data);
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
     Nissan ACC button subscribers:
     */
    void callbackAccButtonRequest(const std_msgs::UInt8::ConstPtr& msg)
    {
        msgBusyButton.data = true;
        publisherBusySendingButtonPress.publish(msgBusyButton);
        ROS_INFO("vehicle_interface Received button request: %d: %s", (int)msg->data, nissanButtonToStr((Panda::NissanButton)msg->data));

        nissanAccButtonHandler->sendButton((Panda::NissanButton)msg->data);

        ROS_INFO("vehicle_interface Done pressing button:    %d: %s", (int)msg->data, nissanButtonToStr((Panda::NissanButton)msg->data));
        msgBusyButton.data = false;
        publisherBusySendingButtonPress.publish(msgBusyButton);
    }

    void callbackControlRequest(const std_msgs::Bool::ConstPtr& msg)
    {
        nissanAccButtonHandler->requestControl(msg->data);
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
            subscribers[1] = n_->subscribe("/car/libpanda/control_request", 1000, &Control::callbackControlRequest, this);
            publisherBusySendingButtonPress = n_->advertise<std_msgs::Bool>("/car/libpanda/busy_sending_button_press", 1000);
        } else {
            toyotaHandler = static_cast<Panda::ToyotaHandler*>(pandaController);
            subscribers[0] = n_->subscribe("/car/cruise/accel_input", 1000, &Control::callbackAccelInput, this);
            subscribers[1] = n_->subscribe("/car/hud/mini_car_enable", 1000, &Control::callbackMiniCar, this);
            subscribers[2] = n_->subscribe("/car/hud/cruise_cancel_request", 1000, &Control::callbackCruiseCancel, this);
            subscribers[3] = n_->subscribe("/car/steer/tq", 1000, &Control::callbackSteerTq, this);
        }
    }

    ~Control(){

    }
};

class ExampleSteeringLimitListener: public Panda::SteeringLimiterListener {

private:
    ros::NodeHandle* nodeHandle;
    ros::Publisher publishSteeringLimit;
    void steeringLimitNotification( Panda::STEERING_STATE value) {
        // std::cout << "New steering limit notification: " << (int)value << std::endl;
        std_msgs::UInt8 msgSteerLimit;
        msgSteerLimit.data = (int)value;
        publishSteeringLimit.publish(msgSteerLimit);
    }
public:
    ExampleSteeringLimitListener(ros::NodeHandle* nodeHandle, Panda::Controller* controller) {
        this->nodeHandle = nodeHandle;

        if (controller->getPandaHandler()->getVehicleManufacturer() == Panda::VEHICLE_MANUFACTURE_TOYOTA) {
            // This is nasty:
            Panda::ToyotaHandler* toyotaHandler = static_cast<Panda::ToyotaHandler*>(controller);
            toyotaHandler->addSteeringTorqueLimiterListener(this);

            publishSteeringLimit = nodeHandle->advertise<std_msgs::UInt8>("/car/libpanda/steer_limiter_state", 1000);

            // Send inital value
            std_msgs::UInt8 msgSteerLimit;
            msgSteerLimit.data = (int)Panda::STEERING_STATE_OK;
            publishSteeringLimit.publish(msgSteerLimit);

        }

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
                ||(canData->messageID==740&&canData->dataLength==5)
                ||(canData->messageID==608&&canData->dataLength==8)
                ||(canData->messageID==36&&canData->dataLength==8)
                ||(canData->messageID==170&&canData->dataLength==8)
                ||(canData->messageID==956&&canData->dataLength==8)
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
                ||(canData->messageID==381&&canData->dataLength==64)
                ||(canData->messageID==382&&canData->dataLength==64)
                ||(canData->messageID==385&&canData->dataLength==64)
                ||(canData->messageID==386&&canData->dataLength==64)
                ||(canData->messageID==389&&canData->dataLength==64)
                ||(canData->messageID==390&&canData->dataLength==64)
                ||(canData->messageID==393&&canData->dataLength==64)
                ||(canData->messageID==394&&canData->dataLength==64)
                ||(canData->messageID==398&&canData->dataLength==64)
                ||(canData->messageID==399&&canData->dataLength==64)
                ||(canData->messageID==405&&canData->dataLength==64)
                ||(canData->messageID==407&&canData->dataLength==64)
                ||(canData->messageID==411&&canData->dataLength==64)
                ||(canData->messageID==412&&canData->dataLength==64)
                ||(canData->messageID==415&&canData->dataLength==64)
                ||(canData->messageID==416&&canData->dataLength==64)
                ||(canData->messageID==419&&canData->dataLength==64)
                ||(canData->messageID==420&&canData->dataLength==64)
                ||(canData->messageID==423&&canData->dataLength==64)
                ||(canData->messageID==425&&canData->dataLength==64)

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
        pub_ = nh1->advertise<std_msgs::String>("/car/can/raw", 1000);
        //		publisherCarSetSpeed = nh1->advertise<std_msgs::UInt8>("/car/cruise/ui_set_speed", 1000);
        //		publisherPandaControlsEnabled = nh1->advertise<std_msgs::String>("/panda/controls_enabled", 1000);

        //FIXME: use libpanda to create CAN and GPS files
        // csvfile.open(filename);
        // csvfile <<"Time" << ","<< "Bus" << "," << "MessageID" << "," << "Message" << ","<< "MessageLength" << std::endl;

    }

};


class PublishGpsObserver : public Panda::GpsListener, public Mogi::Thread {
private:
    ros::NodeHandle nhPublishGps;
    ros::Publisher pub_fix;
    ros::Publisher pub_heading;
    ros::Publisher pub_gpstime;
    Panda::Gps* mGps;
    char navMsgCov[100];
    float cov[9]; // East, North, Up, row major order

    void newDataNotification( Panda::GpsData* gpsData ) {
        time_t gpsTime_t = mktime(&gpsData->time);

        sensor_msgs::NavSatFix fix_position;
        sensor_msgs::TimeReference fix_time;
        ros::Time current_time = ros::Time::now();

        fix_position.header.stamp = current_time;
        fix_position.header.frame_id = "base_link";
        fix_position.status.status = (gpsData->info.status == 'A' ? sensor_msgs::NavSatStatus::STATUS_FIX : sensor_msgs::NavSatStatus::STATUS_NO_FIX); //Marked as active if we get the active character
        fix_position.status.service = sensor_msgs::NavSatStatus::SERVICE_GPS; //Flagged as normal GPS
        fix_position.latitude = gpsData->pose.latitude;
        fix_position.longitude = gpsData->pose.longitude;
        fix_position.altitude = gpsData->pose.altitude;

//        // Old:
//        double hdop_squared_half_sqrt = sqrt(gpsData->quality.HDOP * gpsData->quality.HDOP / 2.0);
//        double vdop = gpsData->quality.VDOP;
//        double covariance_diagonal[] = {hdop_squared_half_sqrt, hdop_squared_half_sqrt, vdop};
//        for (size_t i = 0; i < 3; i++) {
//            for (size_t j = 0; j < 3; j++) {
//                size_t final_index = (i * 3) + j;
//                fix_position.position_covariance[final_index] = covariance_diagonal[i] * covariance_diagonal[j];
//            }
//        }
//        fix_position.position_covariance_type = sensor_msgs::NavSatFix::COVARIANCE_TYPE_APPROXIMATED; // Approximated as per above.
//
//        // New based on ROS nmea_driver package
//        fix_position.position_covariance[0] = (gpsData->quality.HDOP * gpsData->quality.LatitudeSigmaError)*(gpsData->quality.HDOP * gpsData->quality.LatitudeSigmaError);
//        fix_position.position_covariance[4] = (gpsData->quality.HDOP * gpsData->quality.LongitudeSigmaError)*(gpsData->quality.HDOP * gpsData->quality.LongitudeSigmaError);
//        fix_position.position_covariance[8] = (gpsData->quality.VDOP * gpsData->quality.AltitudeSigmaError)*(gpsData->quality.VDOP * gpsData->quality.AltitudeSigmaError);
//
//        fix_position.position_covariance_type = sensor_msgs::NavSatFix::COVARIANCE_TYPE_APPROXIMATED; // Approximated as per above.

        // Libpanda now supports UBX module full covariance matrix:
        for(int i = 0; i < 9; i++) {
            fix_position.position_covariance[i] = cov[i];
        }
        fix_position.position_covariance_type = sensor_msgs::NavSatFix::COVARIANCE_TYPE_KNOWN; // Hell yeah


        fix_time.time_ref = ros::Time((uint32_t)gpsTime_t, ((uint32_t)gpsData->timeMilliseconds) * 1000000);
        fix_time.header.stamp = current_time;
        fix_time.source = "Libpanda";

        pub_fix.publish(fix_position);
        pub_gpstime.publish(fix_time);
    }

    void newHeadingNotification(Panda::GpsData* gpsData) {
        std_msgs::Float64 msg;
        msg.data = gpsData->motion.course;
        pub_heading.publish(msg);
    }

    void doAction() {   // overload of Mogi::Thread
        usleep(9000);
        mGps->sendUbxCommand(Panda::UBX_CLASS_NAV, Panda::UBX_ID_NAV_COV, 0, NULL);
        while(mGps->busyUbx()) {
            usleep(100);
        }
        int length = mGps->getUbxResponse(navMsgCov);
        if(length > 0) {

            cov[0 + 3*0] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_EE]);  //
            cov[1 + 3*1] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_NN]);  //
            cov[2 + 3*2] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_DD]);  //


            cov[1 + 3*0] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_NE]);  //
            cov[0 + 3*1] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_NE]);  //

            cov[2 + 3*0] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_ED]);  //
            cov[0 + 3*2] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_ED]);  //

            cov[1 + 3*2] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_ND]);  //
            cov[2 + 3*1] = Panda::parseFloat(&navMsgCov[UBX_NAV_COV_POS_ND]);  //
        }
    }
public:
    PublishGpsObserver(Panda::Gps* mGps)
    : mGps(mGps) {
        for(int i = 0; i < 9; i++) {
            cov[i] = nan("");
        }
        pub_fix = nhPublishGps.advertise<sensor_msgs::NavSatFix>("/car/gps/fix", 1000);
        pub_heading = nhPublishGps.advertise<std_msgs::Float64>("/car/gps/heading", 1000);
        pub_gpstime = nhPublishGps.advertise<sensor_msgs::TimeReference>("/car/gps/fix_time", 1000);
    }

};

class PublishGpsActive : public Mogi::Thread {
private:
                  ros::Publisher publisher;
    ros::Rate* publishRate;
    std_msgs::Bool msgGpsActive;

    void doAction() {
        msgGpsActive.data = true;
        publisher.publish( msgGpsActive );

        //		ROS_INFO("vehicle_interface Publishing true to /car/panda/gps_active");

        publishRate->sleep();	// 1Hz
    }
public:
    PublishGpsActive(ros::NodeHandle* nodeHandle) {
        publisher = nodeHandle->advertise<std_msgs::Bool>("/car/panda/gps_active", 1000);

        publishRate = new ros::Rate(1.0);
        this->start();
    }

    ~PublishGpsActive() {
        delete publishRate;
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

    PublishGpsObserver publishGpsTracker(&pandaHandler.getGps());
    pandaHandler.addGpsObserver(publishGpsTracker);

    //	Panda::ToyotaHandler toyotaHandler(&pandaHandler);


    ROS_INFO("Initializing PandaHandler...");
    pandaHandler.initialize();

    ROS_INFO("Initializing pandaController from factory...");
    Panda::ControllerClient* pandaController = new Panda::ControllerClient(pandaHandler);

    publishGpsTracker.start();


    if(pandaController->getController() == NULL) {
        ROS_ERROR("No VIN discovered, unable to build controller handler");

        // Delete the controller and kill the process, this cannot continue
        //		delete pandaController;
        //		exit(EXIT_FAILURE);

        // We can for set the VIN to continue with the following code, but best to just fail everything since somethin aint right:
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

    ROS_INFO("Connecting  Adding Steering LImit LIstener...");
    ExampleSteeringLimitListener myExampleSteeringLimitListener(&nh, pandaController->getController());    // This is only added if toyota is confrimed (below, next if statemenr)

    // Initialize panda and toyota handlers
    //	ROS_INFO("Starting ToyotaHandler...");
    //toyotaHandler.start();


    ROS_INFO("Starting Panda::Controller...");
    //	Control vehicleControl(&toyotaHandler, &nh);
    Control vehicleControl(pandaController->getController(), &nh);
    pandaController->getController()->start();

    writeToFileThenClose(filenameGpsStatus, "0\n");	// state 0: on but time not set



    //  Set the sytem time here:
    if(pandaHandler.getGps().available()) {
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
    } else {
        ROS_ERROR("No avialable GPS! Continuting vehicle_interface without setting system time!");

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

    //	ros::Publisher publisherGpsActive = nh.advertise<std_msgs::Bool>("/car/panda/gps_active", 1000);
    //	ros::Rate mainLoopRate(1); // 1Hz
    //	while(ros::ok()) {
    //		// publish that we are active:
    //		std_msgs::Bool msgGpsActive;
    //		msgGpsActive.data = true;
    //		//publisherGpsActive.publish( msgGpsActive );
    //
    //		// Normal ROS rate limiting:
    //		//mainLoopRate.sleep();
    ////		ros::spinOnce();
    //		ros::spin();
    //	}
    PublishGpsActive mPublishGpsActive(&nh);
    ros::spin();

    // Cleanup:
    publishGpsTracker.stop();
    //	mPandaStatusPublisher.stop();
    //	toyotaHandler.stop();
    pandaController->getController()->stop();
    pandaHandler.stop();
    writeToFileThenClose(filenameGpsStatus, "-1\n");

    return 0;
}
