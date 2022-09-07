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
	cruise_state_pub = n_.advertise<std_msgs::Float64>("cruise_state",1000);
	acc_btns_pub = n_.advertise<std_msgs::Float64>("acc_btns",1000);
	lkas_btn_pub = n_.advertise<std_msgs::Float64>("lkas_btn",1000);
	set_speed_pub = n_.advertise<std_msgs::Float64>("set_speed",1000);
	lane_lines_pub = n_.advertise<std_msgs::Float64>("lane_lines",1000);
	state_signal_pub = n_.advertise<std_msgs::Float64>("state_signal",1000);
	set_distance_pub = n_.advertise<std_msgs::Float64>("set_distance",1000);
	hud_states_pub = n_.advertise<std_msgs::Float64>("hud_states",1000);
	system_on_pub = n_.advertise<std_msgs::Float64>("system_on",1000);

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
	else if (MessageID == 308)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //CRUISE_STATE
		cruise_state_pub.publish(msg1);

	}
	else if (MessageID == 1119)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //ACC_BTNS
		acc_btns_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //LKAS_LDW_ON
		lkas_btn_pub.publish(msg2);

	}
	else if (MessageID == 1487)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //SET_SPEED
		set_speed_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //LANE_LINES
		lane_lines_pub.publish(msg2);

		std_msgs::Float64 msg3;
		msg3.data = data.var3; //STATE_SIGNAL
		state_signal_pub.publish(msg3);

		std_msgs::Float64 msg4;
		msg4.data = data.var4; //SET_DISTANCE
		set_distance_pub.publish(msg4);

		std_msgs::Float64 msg5;
		msg5.data = data.var5; //HUD_STATES
		hud_states_pub.publish(msg5);

		std_msgs::Float64 msg6;
		msg6.data = data.var6; //SYSTEM_ON
		system_on_pub.publish(msg6);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher steering_angle_pub;
	ros::Publisher vel_pub;
	ros::Publisher cruise_state_pub;
	ros::Publisher acc_btns_pub;
	ros::Publisher lkas_btn_pub;
	ros::Publisher set_speed_pub;
	ros::Publisher lane_lines_pub;
	ros::Publisher state_signal_pub;
	ros::Publisher set_distance_pub;
	ros::Publisher hud_states_pub;
	ros::Publisher system_on_pub;

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