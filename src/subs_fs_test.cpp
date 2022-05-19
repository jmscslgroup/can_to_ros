#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode_test.h"
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
	steering_angle_pub = n_.advertise<std_msgs::Float64>("steering_angle",1000);
	vel_pub = n_.advertise<std_msgs::Float64>("vel",1000);

	sub_ = n_.subscribe("/realtime_raw_data", 1000, &SubscribeAndPublish::callback, this);
}
	void callback(const std_msgs::String::ConstPtr& raw_data)
{

	std::stringstream ss(raw_data->data);
	ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

	if (MessageID == 139){
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //STEER_ANGLE
		steering_angle_pub.publish(msg1);

	}
	else if (MessageID == 303)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //SPEED
		vel_pub.publish(msg1);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher steering_angle_pub;
	ros::Publisher vel_pub;

	ros::Subscriber sub_;
	decode_msgs obj;
	std::string Time,Buffer,Message,MessageLength;
	double MessageID, Bus;
	values data;
};
int main(int argc, char **argv){
	ros::init(argc, argv, "subs_fs_test");
	ros::NodeHandle nh1;
	SubscribeAndPublish SAPObject;
	ros::spin();
	return 0;
}
