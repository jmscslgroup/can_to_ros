#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include <std_msgs/Float64.h>

using namespace std;



class TemperatureSensor
{
public:
    TemperatureSensor(ros::NodeHandle *nh)
    {
        // Initialize temperature and ROS publisher
        temperature = 0.0;
        raw_data_pubisher =
            nh->advertise<geometry_msgs::PointStamped>("/test_topic", 1000);
    }
    string readTemperatureSensorData()
    {
            std::ifstream inFile;
        std::string user_input="";
        std::string inputLine="";
        int MessageID;

        bool firstLine=true;
        if (argc != 2)
        { // check the nunber of the argument
            std::cout <<"./a.out .csv" << std::endl;
            return 1;
        }

        inFile.open(argv[1]);
        if( !inFile.is_open())
        {// check id the file is opened correctly.
        std::cout << "Cannot open file to read"<< std::endl;
        return 1;
        }
        if (!getline(inFile, inputLine)) break;
                if (firstLine){ // skip the first line
                    firstLine=false;
                    continue;
                }
                if (inputLine.empty()) continue; // if there is an empty line then skip it

                std::replace(inputLine.begin(), inputLine.end(), ',', ' '); // replace the commas with white space
                raw_can=inputLine;
                return raw_can;
    }
    void publishData()
    {
        std_msgs::Float64 msg;
        msg.data = readTemperatureSensorData();
        raw_data_pubisher.publish(msg);
    }
private:
    std::string raw_can;
    ros::Publisher raw_data_pubisher;
};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "proto");
    ros::NodeHandle nh;
    // Create an instance of Temperature sensor
    TemperatureSensor temperatureSensor(&nh);
    // Create a ROS timer for reading data
    // the callback function for the Timer must
    // be bound with std::bind or boost::bind
    ros::Timer timerReadTemperature =
        nh.createTimer(ros::Duration(1.0 / 10.0),
                       std::bind(&TemperatureSensor::readTemperatureSensorData, temperatureSensor));
    // Create a ROS timer for publishing temperature
    ros::Timer timerPublishTemperature =
        nh.createTimer(ros::Duration(1.0 / 10.0),
                       std::bind(&TemperatureSensor::publishData, temperatureSensor));
    // We can now use spin, or do whatever 
    // we want in this node
    ros::spin();
}