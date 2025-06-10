#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Point.h"
//#include "can_to_ros/headlights.h"
#include "std_msgs/UInt8.h"

#include <map>
#include <string>
#include <bitset>
#include <iostream>


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    for (int track = 1; track <= 10; track++) {
        for (int subtrack = 1; subtrack <= 6; subtrack++) {
            std::string side = "L";
            std::string track_name = "car/radar/track_" + side + std::to_string(track) + "_" + std::to_string(subtrack);
            nissan_radar_publishers[track_name] = n_.advertise<geometry_msgs::PointStamped>(track_name, 1000);

            side = "R";
            track_name = "car/radar/track_" + side + std::to_string(track) + "_" + std::to_string(subtrack);
            nissan_radar_publishers[track_name] = n_.advertise<geometry_msgs::PointStamped>(track_name, 1000);
        }
    }

    std::vector<std::string> keys;
    for (const auto& kv : nissan_radar_publishers) {
        keys.push_back(kv.first);
    }

    for (const auto& key : keys) std::cout << key << " ";
    std::cout << std::endl;


    //Topic you want to subscribe
    sub_ = n_.subscribe("/car/can/raw", 1000, &SubscribeAndPublish::callback, this);
  }

  float decode(std::string binary_val, int start, int length, bool negative_flag) {
    std::string raw1 = binary_val.substr(start, length);
    if (negative_flag) {
        raw1 = obj.findTwosComplement(raw1);
    }
    unsigned long long raw_dec = std::stoull(raw1, 0, 2);
	float scaled = (float)raw_dec * 1.00000 + 0.000000;
    return scaled;
  }

  void callback(const std_msgs::String::ConstPtr& raw_data)
  {

    std::stringstream ss(raw_data->data);

    ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

    if (!(MessageID >= 381 && MessageID <= 425)) {
        return;
    }

    int left_message_ids[10] = {381, 385, 389, 393, 398, 405, 411, 415, 419, 423};
    int right_message_ids[10] = {382, 386, 390, 394, 399, 407, 412, 416, 420, 425};

    std::string side = "L";
    for (int i = 0; i < 10; i++) {
        if (right_message_ids[i] == MessageID) {
            side = "R";
            break;
        }
    }

    std::map<int, int> message_id_to_track = {
        {381, 1},
        {382, 1},
        {385, 2},
        {386, 2},
        {389, 3},
        {390, 3},
        {393, 4},
        {394, 4},
        {398, 5},
        {399, 5},
        {405, 6},
        {407, 6},
        {411, 7},
        {412, 7},
        {415, 8},
        {416, 8},
        {419, 9},
        {420, 9},
        {423, 10},
        {425, 10}
    };

    std::map<int, int> subtrack_to_offset = {
        {1, 0},
        {2, 72},
        {3, 72 * 2},
        {4, 72 * 3},
        {5, 73 * 4},
        {6, 72 * 5}
    };

    int len = Message.length();
    int i = 0;
    std::string word;
    std::string binary;
    unsigned long long n;

    binary.clear();
    for (char c : Message) {
        unsigned int val;
        std::stringstream ss;
        ss << std::hex << c;
        ss >> val;
        binary.append(std::bitset<4>(val).to_string());
    }


    for (int subtrack = 1; subtrack <= 6; subtrack++) {
        std::string track_name = "car/radar/track_" + side + std::to_string(message_id_to_track[MessageID]) + "_" + std::to_string(subtrack);
        
        int offset = subtrack_to_offset[subtrack];

        double long_data = decode(binary, 99 + offset, 13, false); // DISTANCE_B_#
        double lat_data = decode(binary, 85 + offset, 13, true); // DISTANCE_A_#
        double valid_data = decode(binary, 113 + offset, 1, false); // VALID_A_#

        geometry_msgs::PointStamped marker;
        marker.header.frame_id = "front_laser_link";
        marker.header.stamp = ros::Time(std::stod(Time));

        marker.point.x = long_data; // long
        marker.point.y = lat_data; // lat
        marker.point.z = valid_data; // rel speed

        nissan_radar_publishers[track_name].publish(marker);
    }

}

private:
    ros::NodeHandle n_;

    std::map<std::string, ros::Publisher> nissan_radar_publishers;

    ros::Subscriber sub_;
    decode_msgs obj;
    std::string Time,Buffer,Message,MessageLength;
    double MessageID, Bus;
    values data;

};//End of class SubscribeAndPublish

// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "subs_nissan_side_radar");
    ros::NodeHandle nh1;
    // ROS_INFO("Got parameter");

    SubscribeAndPublish SAPObject;
     ros::spin();

  return 0;
}
