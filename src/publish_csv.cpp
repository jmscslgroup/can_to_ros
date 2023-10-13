#include <getopt.h>

#include "ros/ros.h"
#include "std_msgs/String.h"
//#include "geometry_msgs/Point.h"
//#include "geometry_msgs/Twist.h"
//#include "geometry_msgs/PointStamped.h"
#include "sensor_msgs/NavSatFix.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
// #include "can_to_ros/can_msgs.h"

void printUsage(const char* argv0) {
    ROS_ERROR("Please provide a GPS and/or CAN file: ");
    ROS_ERROR("  %s [-c canFile] [-g gpsFile]", argv0);
}


class HighlySpecificFileReader {
private:
    std::ifstream file;
    std::string latestLine;
    ros::Time fileTime;
    
    bool haveFileTimeStart;
    ros::Time fileTimeStart;
    
    bool fileComplete;
    
public:
    HighlySpecificFileReader ()
    : haveFileTimeStart(false), fileComplete(false) {
        
    }
    
    int readNextLine () {
        if (!getline(file, latestLine)) {   // EOF
            fileComplete = true;
            return -1;
        }
        
        
        if (latestLine.empty()) {
            ROS_WARN("Empty line in file!");
            return 1; // if there is an empty line then skip it
        }
        
        std::replace(latestLine.begin(), latestLine.end(), ',', ' '); // replace the commas with white space
        
        sscanf(latestLine.c_str(),"%d.%d", &fileTime.sec, &fileTime.nsec);
        
        fileTime.nsec *= 1000;
        //        ROS_INFO("fileTime = %d.%09d", fileTime.sec, fileTime.nsec);
        if(!haveFileTimeStart) {
            fileTimeStart = fileTime;
            haveFileTimeStart = true;
        }
        //        currentFileTime = fileTime.toSec();
        
        //        canMsg.data = latestLine;
        
        return 0;
    }
    
    bool isComplete() {
        return fileComplete;
    }
    
    void setStartTime(const ros::Time& startTime) {
        fileTimeStart = startTime;
//        haveFileTimeStart = true;
    }
    
    const ros::Time& latestTime() {
        return fileTime;
    }
    double runningTime() {
        return (fileTime - fileTimeStart).toSec();
    }
    const std::string& getLatestLine() {
        return latestLine;
    }
    
    int load(std::string fileName) {
        file.open(fileName.c_str());
        if( !file.is_open())
        {
            ROS_ERROR("Cannot open file \"%s\"", fileName.c_str());
            return 1;
        }
        getline(file, latestLine);// Skip first line (header)
        readNextLine(); // get lastest string and file time
        return 0;
    }
};


int main(int argc, char **argv){ 
    ros::init(argc, argv, "publish_csv", ros::init_options::AnonymousName);
    ros::NodeHandle nh1("~");
    
    ros::Publisher publisherCan;// = nh1.advertise<std_msgs::String>("/car/can/raw", 1000);
    ros::Publisher publisherGps;// = nh1.advertise<std_msgs::String>("/car/gps/fix", 1000);
    
    bool canFileProvided = false;
    bool gpsFileProvided = false;
    
    std::string canFileName, gpsFileName;
    
    option longopts[] = {
        {"gps", optional_argument, NULL, 'g'},
        {"can", optional_argument, NULL, 'c'},
        {NULL, 0, NULL, 0}};
    
    while(true) {
        const int opt = getopt_long(argc, argv, "c:g:", longopts, 0);
        
        if (opt == -1) {
            break;
        }
        
        switch (opt) {
            case 'c':
                canFileProvided = true;
                canFileName = optarg;
                break;
            case 'g':
                gpsFileProvided = true;
                gpsFileName = optarg;
                break;
            default:
                printUsage(argv[0]);
                return 1;
        }
    }
    
    if(!canFileProvided && !gpsFileProvided) {
        printUsage(argv[0]);
        return 1;
    }
    
    if(canFileProvided)
        ROS_INFO(" - Got CAN file: %s", canFileName.c_str());
    if(gpsFileProvided)
        ROS_INFO(" - Got GPS file: %s", gpsFileName.c_str());
    
    //    std::ifstream canFile, gpsFile;
    //    std::string user_input="";
    //    std::string inputLine="";
    //    std::string Time,Buffer,Message,MessageLength;
    double MessageID, Bus;
    
    HighlySpecificFileReader mGpsHighlySpecificFileReader;
    HighlySpecificFileReader mCanHighlySpecificFileReader;
    
    ros::Time canTimeStart;
    ros::Time gpsTimeStart;
    
    if(canFileProvided) {
        if(mCanHighlySpecificFileReader.load(canFileName.c_str())) {
            ROS_ERROR("Cannot open file \"%s\"", canFileName.c_str());
            return 1;
        }
        canTimeStart = mCanHighlySpecificFileReader.latestTime();
        publisherCan = nh1.advertise<std_msgs::String>("/car/can/raw", 1000);
    }
    if(gpsFileProvided) {
        if(mGpsHighlySpecificFileReader.load(gpsFileName.c_str())) {
            ROS_ERROR("Cannot open file \"%s\"", gpsFileName.c_str());
            return 1;
        }
        gpsTimeStart = mCanHighlySpecificFileReader.latestTime();
        publisherGps = nh1.advertise<sensor_msgs::NavSatFix>("/car/gps/fix", 1000);
    }
    
    
    // Synchronize time calculations between files if both provided:
    if(gpsFileProvided && canFileProvided) {
        if(mGpsHighlySpecificFileReader.latestTime().toSec() > mCanHighlySpecificFileReader.latestTime().toSec()) {
            mGpsHighlySpecificFileReader.setStartTime(mCanHighlySpecificFileReader.latestTime());
        } else {
            mCanHighlySpecificFileReader.setStartTime(mGpsHighlySpecificFileReader.latestTime());
        }
    }
    
    int totalPublishedCanMessages = 0;
    int totalPublishedGpsMessages = 0;
    ros::Rate rate(10000.0); // This is abritrary, just made VERY fast=
    
    std_msgs::String canMsg;
    sensor_msgs::NavSatFix gpsMsg;
    sensor_msgs::TimeReference gpsFixTime;
    ros::Time startTime = ros::Time::now();
    double runningTime; // - current time - start time;
    double nextPublishTime = 0; // publish first message immediately
    
    bool canShouldPublish;
    
    while (ros::ok()){  //ros::ok()
        
        if(gpsFileProvided && canFileProvided) {
            if(!mCanHighlySpecificFileReader.isComplete() && mGpsHighlySpecificFileReader.isComplete()) {
                canShouldPublish = true;
                nextPublishTime = mCanHighlySpecificFileReader.runningTime();
            } else if(mCanHighlySpecificFileReader.isComplete() && !mGpsHighlySpecificFileReader.isComplete()) {
                canShouldPublish = false;
                nextPublishTime = mGpsHighlySpecificFileReader.runningTime();
            } else if(mCanHighlySpecificFileReader.runningTime() < mGpsHighlySpecificFileReader.runningTime()) {
                canShouldPublish = true;
                nextPublishTime = mCanHighlySpecificFileReader.runningTime();
            } else {
                canShouldPublish = false;
                nextPublishTime = mGpsHighlySpecificFileReader.runningTime();
            }
        } else if(canFileProvided) {
            nextPublishTime = mCanHighlySpecificFileReader.runningTime();
            canShouldPublish = true;
        } else {    // only GPS file
            nextPublishTime = mGpsHighlySpecificFileReader.runningTime();
            canShouldPublish = false;
        }
        
        runningTime = (ros::Time::now() - startTime).toSec();
//        printf("Next time: %f current Time %f CAN msgs: %07d GPS msgs: %07d \r", nextPublishTime, runningTime, totalPublishedCanMessages, totalPublishedGpsMessages);
        while( runningTime < nextPublishTime ) {
//            printf("Total Published CAN messages: % 7d GPS Messages: % 6d\r", totalPublishedCanMessages, totalPublishedGpsMessages);
  //          printf("Next time: %f current Time %f\r", nextPublishTime, runningTime);
            rate.sleep();
            runningTime = (ros::Time::now() - startTime).toSec();
        }
        
        if(canShouldPublish) {
            canMsg.data = mCanHighlySpecificFileReader.getLatestLine();
            publisherCan.publish(canMsg);
            totalPublishedCanMessages++;
            
            mCanHighlySpecificFileReader.readNextLine();    // Read the CSV's next line
        } else {
            std::string gpsLine = mGpsHighlySpecificFileReader.getLatestLine();
            
            int time1, time2;
            char status;
            double HDOP,PDOP,VDOP;
//            ROS_INFO(gpsLine.c_str());
            sscanf(gpsLine.c_str(),"%d.%d %c %lf %lf %lf %lf %lf %lf", &time1, &time2, &status, &gpsMsg.longitude, &gpsMsg.latitude, &gpsMsg.altitude, &HDOP, &PDOP, &VDOP);
            
            gpsMsg.header.stamp = ros::Time::now();
            gpsMsg.header.frame_id = "base_link";
            gpsMsg.status.status = (status == 'A' ? sensor_msgs::NavSatStatus::STATUS_FIX : sensor_msgs::NavSatStatus::STATUS_NO_FIX);
            gpsMsg.status.service = sensor_msgs::NavSatStatus::SERVICE_GPS;
            
            // Sased on ROS nmea_driver package:
            double LatitudeSigmaError = 4;
            double LongitudeSigmaError = 4;
            double AltitudeSigmaError = 4;
            gpsMsg.position_covariance[0] = (HDOP * LatitudeSigmaError)*(HDOP * LatitudeSigmaError);
            gpsMsg.position_covariance[4] = (HDOP * LongitudeSigmaError)*(HDOP * LongitudeSigmaError);
            gpsMsg.position_covariance[8] = (VDOP * AltitudeSigmaError)*(VDOP * AltitudeSigmaError);
            
            gpsMsg.position_covariance_type = sensor_msgs::NavSatFix::COVARIANCE_TYPE_APPROXIMATED; // Approximated as per above.
            
            publisherGps.publish(gpsMsg);
            totalPublishedGpsMessages++;
            
            mGpsHighlySpecificFileReader.readNextLine();    // Read the CSV's next line
        }
        
        if(gpsFileProvided && canFileProvided) {
            if(mGpsHighlySpecificFileReader.isComplete() && mCanHighlySpecificFileReader.isComplete()) {
                break;
            }
        } else if (canFileProvided) {
            if(mCanHighlySpecificFileReader.isComplete()) {
                break;
            }
        } else {
            if(mGpsHighlySpecificFileReader.isComplete()) {
                break;
            }
        }
        
        //rate.sleep();
        
    }
    
    ROS_INFO("Finish publishing CSV files");
    
    return 0;
}
