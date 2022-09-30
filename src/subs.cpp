#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int8.h"
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
	lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist",1000);
	rel_vel_pub = n_.advertise<std_msgs::Float64>("rel_vel",1000);
	track_a0_pub = n_.advertise<geometry_msgs::PointStamped>("track_a0",1000);
	track_a1_pub = n_.advertise<geometry_msgs::PointStamped>("track_a1",1000);
	track_a2_pub = n_.advertise<geometry_msgs::PointStamped>("track_a2",1000);
	track_a3_pub = n_.advertise<geometry_msgs::PointStamped>("track_a3",1000);
	track_a4_pub = n_.advertise<geometry_msgs::PointStamped>("track_a4",1000);
	track_a5_pub = n_.advertise<geometry_msgs::PointStamped>("track_a5",1000);
	track_a6_pub = n_.advertise<geometry_msgs::PointStamped>("track_a6",1000);
	track_a7_pub = n_.advertise<geometry_msgs::PointStamped>("track_a7",1000);
	track_a8_pub = n_.advertise<geometry_msgs::PointStamped>("track_a8",1000);
	track_a9_pub = n_.advertise<geometry_msgs::PointStamped>("track_a9",1000);
	track_a10_pub = n_.advertise<geometry_msgs::PointStamped>("track_a10",1000);
	track_a11_pub = n_.advertise<geometry_msgs::PointStamped>("track_a11",1000);
	track_a12_pub = n_.advertise<geometry_msgs::PointStamped>("track_a12",1000);
	track_a13_pub = n_.advertise<geometry_msgs::PointStamped>("track_a13",1000);
	track_a14_pub = n_.advertise<geometry_msgs::PointStamped>("track_a14",1000);
	track_a15_pub = n_.advertise<geometry_msgs::PointStamped>("track_a15",1000);
	highbeams_pub = n_.advertise<std_msgs::Float64>("highbeams",1000);
	pcm_cruise_2_pub = n_.advertise<geometry_msgs::Point>("pcm_cruise_2",1000);
	acc_accel_cmd_pub = n_.advertise<std_msgs::Float64>("acc/accel_cmd",1000);
	acc_acc_info_pub = n_.advertise<geometry_msgs::Point>("acc/acc_info",1000);
	acc_acc_cut_in_pub = n_.advertise<std_msgs::Int16>("acc/acc_cut_in",1000);
	acc_acc_malfunction_pub = n_.advertise<std_msgs::Int16>("acc/acc_malfunction",1000);
	cruise_state_pub = n_.advertise<geometry_msgs::Point>("cruise_state",1000);
	acc_distance_setting_pub = n_.advertise<std_msgs::Int16>("acc/distance_setting",1000);
	acc_set_speed_pub = n_.advertise<std_msgs::Int16>("acc/set_speed",1000);
	acc_set_speed2_pub = n_.advertise<std_msgs::Float64>("acc/set_speed2",1000);
	accel_pub = n_.advertise<std_msgs::Float64>("accel",1000);

	sub_ = n_.subscribe("/realtime_raw_data", 1000, &SubscribeAndPublish::callback, this);
}
	void callback(const std_msgs::String::ConstPtr& raw_data)
{

	std::stringstream ss(raw_data->data);
	ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

	if (MessageID == 37){
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //STEER_ANGLE
		steering_angle_pub.publish(msg1);

	}
	else if (MessageID == 180)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //SPEED
		vel_pub.publish(msg1);

	}
	else if (MessageID == 869)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //LEAD_DISTANCE
		lead_dist_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //REL_SPEED
		rel_vel_pub.publish(msg2);

	}
	else if (MessageID == 384)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a0_pub.publish(msg1);

	}
	else if (MessageID == 385)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a1_pub.publish(msg1);

	}
	else if (MessageID == 386)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a2_pub.publish(msg1);

	}
	else if (MessageID == 387)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a3_pub.publish(msg1);

	}
	else if (MessageID == 388)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a4_pub.publish(msg1);

	}
	else if (MessageID == 389)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a5_pub.publish(msg1);

	}
	else if (MessageID == 390)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a6_pub.publish(msg1);

	}
	else if (MessageID == 391)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a7_pub.publish(msg1);

	}
	else if (MessageID == 392)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a8_pub.publish(msg1);

	}
	else if (MessageID == 393)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a9_pub.publish(msg1);

	}
	else if (MessageID == 394)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a10_pub.publish(msg1);

	}
	else if (MessageID == 395)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a11_pub.publish(msg1);

	}
	else if (MessageID == 396)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a12_pub.publish(msg1);

	}
	else if (MessageID == 397)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a13_pub.publish(msg1);

	}
	else if (MessageID == 398)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a14_pub.publish(msg1);

	}
	else if (MessageID == 399)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
    msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LONG_DIST
    msg1.point.y = data.var2; //LAT_DIST
    msg1.point.z = data.var3; //REL_SPEED
		track_a15_pub.publish(msg1);

	}
	else if (MessageID == 1570)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //HIGH_BEAMS_ON
		highbeams_pub.publish(msg1);

	}
	else if (MessageID == 467)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::Point msg1;
		msg1.x = data.var1; //MAIN_ON
		msg1.y = data.var2; //SET_SPEED
		pcm_cruise_2_pub.publish(msg1);

	}
	else if (MessageID == 835)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //ACCEL_CMD
		acc_accel_cmd_pub.publish(msg1);

		geometry_msgs::Point msg2;
		msg2.x = data.var2; //MINI_CAR
		msg2.y = data.var3; //CAR_AHEAD
		msg2.z = data.var4; //CANCEL_REQ
		acc_acc_info_pub.publish(msg2);

		std_msgs::Int16 msg3;
		msg3.data = data.var5; //ACC_CUT_IN
		acc_acc_cut_in_pub.publish(msg3);

		std_msgs::Int16 msg4;
		msg4.data = data.var6; //ACC_MALFUNCTION
		acc_acc_malfunction_pub.publish(msg4);

	}
	else if (MessageID == 921)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::Point msg1;
		msg1.x = data.var1; //MAIN_ON
		msg1.y = data.var2; //UI_SET_SPEED
		msg1.z = data.var3; //CRUISE_CONTROL_STATE
		cruise_state_pub.publish(msg1);

		std_msgs::Int16 msg2;
		msg2.data = data.var4; //DISTANCE_LINES
		acc_distance_setting_pub.publish(msg2);

		std_msgs::Int16 msg3;
		msg3.data = data.var2; //UI_SET_SPEED
		acc_set_speed_pub.publish(msg3);

		std_msgs::Float64 msg4;
		msg4.data = data.var2; //UI_SET_SPEED
		acc_set_speed2_pub.publish(msg4);

	}
	else if (MessageID == 552)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //ACCEL_X
		accel_pub.publish(msg1);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher steering_angle_pub;
	ros::Publisher vel_pub;
	ros::Publisher lead_dist_pub;
	ros::Publisher rel_vel_pub;
	ros::Publisher track_a0_pub;
	ros::Publisher track_a1_pub;
	ros::Publisher track_a2_pub;
	ros::Publisher track_a3_pub;
	ros::Publisher track_a4_pub;
	ros::Publisher track_a5_pub;
	ros::Publisher track_a6_pub;
	ros::Publisher track_a7_pub;
	ros::Publisher track_a8_pub;
	ros::Publisher track_a9_pub;
	ros::Publisher track_a10_pub;
	ros::Publisher track_a11_pub;
	ros::Publisher track_a12_pub;
	ros::Publisher track_a13_pub;
	ros::Publisher track_a14_pub;
	ros::Publisher track_a15_pub;
	ros::Publisher highbeams_pub;
	ros::Publisher pcm_cruise_2_pub;
	ros::Publisher acc_accel_cmd_pub;
	ros::Publisher acc_acc_info_pub;
	ros::Publisher acc_acc_cut_in_pub;
	ros::Publisher acc_acc_malfunction_pub;
	ros::Publisher cruise_state_pub;
	ros::Publisher acc_distance_setting_pub;
	ros::Publisher acc_set_speed_pub;
	ros::Publisher acc_set_speed2_pub;
	ros::Publisher accel_pub;

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