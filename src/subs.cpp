#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int8.h"
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
	steering_angle_pub = n_.advertise<std_msgs::Float64>("steering_angle",1000);
	accel_pedal_driver_pub = n_.advertise<std_msgs::Float64>("accel_pedal_driver",1000);
	vel_pub = n_.advertise<std_msgs::Float64>("vel",1000);
	acc_cruise_state_int_pub = n_.advertise<std_msgs::Int16>("acc/cruise_state_int",1000);
	acc_cruise_state_pub = n_.advertise<std_msgs::String>("acc/cruise_state",1000);
	wheel_torque_pub = n_.advertise<std_msgs::Float64>("wheel_torque",1000);
	brake_torque_pub = n_.advertise<std_msgs::Float64>("brake_torque",1000);
	acc_acc_btns_pub = n_.advertise<std_msgs::String>("acc/acc_btns",1000);
	acc_acc_btns_int_pub = n_.advertise<std_msgs::Int16>("acc/acc_btns_int",1000);
	acc_set_speed_pub = n_.advertise<std_msgs::Int16>("acc/set_speed",1000);
	acc_state_signal_pub = n_.advertise<std_msgs::Float64>("acc/state_signal",1000);
	acc_set_distance_pub = n_.advertise<std_msgs::Int16>("acc/set_distance",1000);
	hud_states_pub = n_.advertise<std_msgs::Float64>("hud_states",1000);
	acc_mini_car_pub = n_.advertise<std_msgs::Int16>("acc/mini_car",1000);

	sub_ = n_.subscribe("/car/can/raw", 1000, &SubscribeAndPublish::callback, this);
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
	else if (MessageID == 140)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //ACCEL_PEDAL_POSITION
		accel_pedal_driver_pub.publish(msg1);

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
		std_msgs::Int16 msg1;
		msg1.data = data.var1; //CRUISE_STATE
		acc_cruise_state_int_pub.publish(msg1);

		std_msgs::String msg2;
		msg2.data = data.choice_var1; //CRUISE_STATE
		acc_cruise_state_pub.publish(msg2);

		std_msgs::Float64 msg3;
		msg3.data = data.var2; //WHEEL_TORQUE_CMD
		wheel_torque_pub.publish(msg3);

		std_msgs::Float64 msg4;
		msg4.data = data.var3; //BRAKE_TORQUE_CMD
		brake_torque_pub.publish(msg4);

	}
	else if (MessageID == 1119)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::String msg1;
		msg1.data = data.choice_var1; //ACC_BTNS
		acc_acc_btns_pub.publish(msg1);

		std_msgs::Int16 msg2;
		msg2.data = data.var1; //ACC_BTNS
		acc_acc_btns_int_pub.publish(msg2);

	}
	else if (MessageID == 1487)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Int16 msg1;
		msg1.data = data.var1; //SET_SPEED
		acc_set_speed_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //STATE_SIGNAL
		acc_state_signal_pub.publish(msg2);

		std_msgs::Int16 msg3;
		msg3.data = data.var3; //SET_DISTANCE
		acc_set_distance_pub.publish(msg3);

		std_msgs::Float64 msg4;
		msg4.data = data.var4; //HUD_STATES
		hud_states_pub.publish(msg4);

		std_msgs::Int16 msg5;
		msg5.data = data.var5; //MINICAR
		acc_mini_car_pub.publish(msg5);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher steering_angle_pub;
	ros::Publisher accel_pedal_driver_pub;
	ros::Publisher vel_pub;
	ros::Publisher acc_cruise_state_int_pub;
	ros::Publisher acc_cruise_state_pub;
	ros::Publisher wheel_torque_pub;
	ros::Publisher brake_torque_pub;
	ros::Publisher acc_acc_btns_pub;
	ros::Publisher acc_acc_btns_int_pub;
	ros::Publisher acc_set_speed_pub;
	ros::Publisher acc_state_signal_pub;
	ros::Publisher acc_set_distance_pub;
	ros::Publisher hud_states_pub;
	ros::Publisher acc_mini_car_pub;

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
