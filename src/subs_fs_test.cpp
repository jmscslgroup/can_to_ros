#include "ros/ros.h"
#include "std_msgs/String.h"
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



class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
	vel_pub = n_.advertise<std_msgs::Float64>("vel",1000);
	lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist",1000);
	rel_vel_pub = n_.advertise<std_msgs::Float64>("rel_vel",1000);
	track_a0_pub = n_.advertise<geometry_msgs::PointStamped>("track_a0",1000);
	accel_pub = n_.advertise<std_msgs::Float64>("accel",1000);

	sub_ = n_.subscribe("/realtime_raw_data", 1000, &SubscribeAndPublish::callback, this);
}
	void callback(const std_msgs::String::ConstPtr& raw_data)
{

	std::stringstream ss(raw_data->data);
	ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

	if (MessageID == 180){
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg;
		msg.data = data.var1; //SPEED
		vel_pub.publish(msg);

	}
	else if (MessageID == 869)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg;
		msg.data = data.var1; //LEAD_DISTANCE
		lead_dist_pub.publish(msg);

		std_msgs::Float64 msg;
		msg.data = data.var2; //REL_SPEED
		rel_vel_pub.publish(msg);

	}
	else if (MessageID == 384)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg;
		msg.header.frame_id = "front_laser_link";
                msg.header.stamp = ros::Time(std::stod(Time));
		msg.point.x = data.var1; //LONG_DIST
                msg.point.y = data.var2; //LAT_DIST
                msg.point.z = data.var3; //REL_SPEED
		track_a0_pub.publish(msg);

	}
	else if (MessageID == 552)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg;
		msg.data = data.var1; //ACCEL_X
		accel_pub.publish(msg);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher vel;
	ros::Publisher lead_dist;
	ros::Publisher rel_vel;
	ros::Publisher track_a0;
	ros::Publisher accel;

	ros::Subscriber sub_;
	decode_msgs obj;
	std::string Time,Buffer,Message,MessageLength;
	double MessageID, Bus;
	values data;
};
int main(int argc, char **argv){
	ros::init(argc, argv, "subs_fs");
	ros::NodeHandle nh1;
	SubscribeAndPublish SAPObject;
	ros::spin();
	return 0;
}
