/*
 Author: Matt Bunting
 Copyright (c) 2023 Vanderbilt

 */



/*
 
 topics:
    /ramp_in : Float64
    /ramp_out: Float64
 
 params:
    ramp_max_rate_up: default 1, defines max increase of output
    ramp_max_rate_down: default 1, defines max decrease of output
    ramp_publish_rate: defualt 10Hz, defines output publish rate for potentially idle input
 
 */

#include <sys/time.h> // for SimpleDerivative

// ROS headers:
#include "ros/ros.h"
//#include "std_msgs/Bool.h"
#include "std_msgs/Float64.h"

#include <panda.h>   // leveraging mogi::thread


class Ramp : public Mogi::Thread {
private:
    
    ros::Subscriber subscriber;
    ros::Publisher publisher;
    
    ros::Rate* rosRate;
    
    double maxRateUp;
    double maxRateDown;
    
    double inputState;
    double outputState;
    
    struct timeval priorTime;
    
public:

    
    void callbackNewInput(const std_msgs::Float64::ConstPtr& msg) {
        inputState = msg->data;
        
        process();
    }
    
    void publishOutput() {
        std_msgs::Float64 outputMessage;
        outputMessage.data = outputState;
        
        publisher.publish(outputMessage);
    }
    
    // Mogi::Thread initialization on start() call
    void entryAction() {
        gettimeofday(&priorTime, NULL);
    }
	
    // Mogi::Thread regular execution loop:
    void doAction() {
        process();
//        usleep(100000);
        rosRate->sleep();
    }
    
    void process() {
        // find time difference to last call
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        
        double dTime = (double)(currentTime.tv_sec - priorTime.tv_sec) + ((double)(currentTime.tv_usec - priorTime.tv_usec))/1000000.0;
        priorTime = currentTime;
        
        // check for numerical errors
        if (dTime == 0) {
            return;     // nothing to do
        }
        
        // Ramp up:
        if(outputState < inputState) {
            double amountToTarget = inputState - outputState;
            double maxAmountToIncrease = dTime * maxRateUp;
            if( maxAmountToIncrease < amountToTarget ) {
                outputState += maxAmountToIncrease;
            } else {
//                outputState += amountToIncrease == inputState - outputState ==> outputState=inputState
                outputState = inputState;
            }
        } else if (outputState > inputState) {
            double amountToTarget = inputState - outputState; // will be negative
            double maxAmountToDecrease = -dTime * maxRateDown;  // made to be negative
            if( maxAmountToDecrease > amountToTarget  ) {   // both negative, so swap comarison
                outputState += maxAmountToDecrease;
            } else {
//                outputState += amountToDecrease == inputState - outputState ==> outputState=inputState
                outputState = inputState;
            }
            
        }
        
        publishOutput();
    }
	
	
	
	Ramp(ros::NodeHandle* nodeHandle) {
        
        nodeHandle->param<double>("ramp_max_rate_up", maxRateUp, 1);
        nodeHandle->param<double>("ramp_max_rate_down", maxRateDown, 1);
        if(maxRateUp < 0) {
            maxRateUp = -maxRateUp;
        }
        if(maxRateDown < 0) {
            maxRateDown = -maxRateDown;
        }
        
        double publishRate;
        nodeHandle->param<double>("ramp_publish_rate", publishRate, 10);
        
        inputState = 0;
        outputState = 0;
        
//        maxRateUp = 1;  // TODO: Ros paramify
//        maxRateDown = 1;
        
        rosRate = new ros::Rate(publishRate);    // TODO: Make freq as param
		
		publisher = nodeHandle->advertise<std_msgs::Float64>("ramp_out", 1000);
		subscriber = nodeHandle->subscribe("ramp_in", 1000, &Ramp::callbackNewInput, this);
	}
	
};



int main(int argc, char **argv) {
	ros::init(argc, argv, "ramp");

	ros::NodeHandle nh;
	
	Ramp mRamp(&nh);
    
    
    ROS_INFO("Ramp starting...");
    mRamp.start();
    
    ros::spin();
    
    ROS_INFO("Stopping Ramp...");
    mRamp.stop();
    ROS_INFO("Ramp done.");
	
    return 0;
}
