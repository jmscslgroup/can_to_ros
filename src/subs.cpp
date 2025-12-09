#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Point.h"
//#include "can_to_ros/headlights.h"
#include "std_msgs/UInt8.h"
#include "std_msgs/Float64MultiArray.h"



class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
	car_state_imu_x_y_yaw_pub = n_.advertise<std_msgs::Float64MultiArray>("car/state/imu_x_y_yaw",1000);
	car_state_steering_pub = n_.advertise<std_msgs::Float64MultiArray>("car/state/steering",1000);
	car_state_wheel_speeds_pub = n_.advertise<std_msgs::Float64MultiArray>("car/state/wheel_speeds",1000);
	_car_state_vel_x_pub = n_.advertise<std_msgs::Float64>("/car/state/vel_x",1000);
	car_radar_track_a0_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a0",1000);
	car_radar_track_a1_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a1",1000);
	car_radar_track_a2_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a2",1000);
	car_radar_track_a3_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a3",1000);
	car_radar_track_a4_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a4",1000);
	car_radar_track_a5_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a5",1000);
	car_radar_track_a6_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a6",1000);
	car_radar_track_a7_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a7",1000);
	car_radar_track_a8_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a8",1000);
	car_radar_track_a9_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a9",1000);
	car_radar_track_a10_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a10",1000);
	car_radar_track_a11_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a11",1000);
	car_radar_track_a12_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a12",1000);
	car_radar_track_a13_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a13",1000);
	car_radar_track_a14_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a14",1000);
	car_radar_track_a15_pub = n_.advertise<geometry_msgs::PointStamped>("car/radar/track_a15",1000);
	pcm_cruise_2_pub = n_.advertise<geometry_msgs::Point>("pcm_cruise_2",1000);
	acc_set_speed_pub = n_.advertise<std_msgs::Int16>("acc/set_speed",1000);
	car_state_imu_x_pub = n_.advertise<std_msgs::Float64MultiArray>("car/state/imu_x",1000);
	steer_torque_driver_pub = n_.advertise<std_msgs::Float64>("steer_torque_driver",1000);
	steer_torque_eps_pub = n_.advertise<std_msgs::Float64>("steer_torque_eps",1000);
	steer_override_pub = n_.advertise<std_msgs::Int16>("steer_override",1000);
	steer_request_pub = n_.advertise<std_msgs::Int16>("steer_request",1000);
	steer_torque_cmd_pub = n_.advertise<std_msgs::Float64>("steer_torque_cmd",1000);
	acc_accel_cmd_pub = n_.advertise<std_msgs::Float64>("acc/accel_cmd",1000);
	acc_acc_info_pub = n_.advertise<geometry_msgs::Point>("acc/acc_info",1000);
	acc_mini_car_pub = n_.advertise<std_msgs::Int16>("acc/mini_car",1000);
	acc_acc_cut_in_pub = n_.advertise<std_msgs::Int16>("acc/acc_cut_in",1000);
	acc_acc_malfunction_pub = n_.advertise<std_msgs::Int16>("acc/acc_malfunction",1000);
	lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist",1000);
	rel_vel_pub = n_.advertise<std_msgs::Float64>("rel_vel",1000);
	acc_set_btn_pub = n_.advertise<std_msgs::Int16>("acc/set_btn",1000);
	acc_res_btn_pub = n_.advertise<std_msgs::Int16>("acc/res_btn",1000);
	acc_cancel_btn_pub = n_.advertise<std_msgs::Int16>("acc/cancel_btn",1000);
	cruise_state_pub = n_.advertise<geometry_msgs::Point>("cruise_state",1000);
	acc_set_distance_pub = n_.advertise<std_msgs::Int16>("acc/set_distance",1000);
	acc_cruise_state_pub = n_.advertise<std_msgs::String>("acc/cruise_state",1000);
	acc_cruise_state_int_pub = n_.advertise<std_msgs::Int16>("acc/cruise_state_int",1000);
	acc_set_speed2_pub = n_.advertise<std_msgs::Float64>("acc/set_speed2",1000);
	car_state_sport_mode_pub = n_.advertise<std_msgs::Bool>("car/state/sport_mode",1000);
	car_state_eco_mode_pub = n_.advertise<std_msgs::Bool>("car/state/eco_mode",1000);
	highbeams_pub = n_.advertise<std_msgs::Float64>("highbeams",1000);

	sub_ = n_.subscribe("/car/can/raw", 1000, &SubscribeAndPublish::callback, this);
}
	void callback(const std_msgs::String::ConstPtr& raw_data)
{

	std::stringstream ss(raw_data->data);
	ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

	if (MessageID == 36){
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64MultiArray msg1;
		msg1.layout.dim.push_back(std_msgs::MultiArrayDimension());
                msg1.layout.dim[0].size = 3;
                msg1.layout.dim[0].stride = 1;
                msg1.layout.dim[0].label = "ACCEL_X/ACCEL_Y/YAW_RATE";
                
                msg1.data.clear();
		msg1.data.push_back(data.var1);	//ACCEL_X
		msg1.data.push_back(data.var2);	//ACCEL_Y
		msg1.data.push_back(data.var3);	//YAW_RATE
		car_state_imu_x_y_yaw_pub.publish(msg1);

	}
	else if (MessageID == 37)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64MultiArray msg1;
		msg1.layout.dim.push_back(std_msgs::MultiArrayDimension());
                msg1.layout.dim[0].size = 3;
                msg1.layout.dim[0].stride = 1;
                msg1.layout.dim[0].label = "STEER_ANGLE/STEER_FRACTION/STEER_RATE";
                
                msg1.data.clear();
		msg1.data.push_back(data.var1);	//STEER_ANGLE
		msg1.data.push_back(data.var2);	//STEER_FRACTION
		msg1.data.push_back(data.var3);	//STEER_RATE
		car_state_steering_pub.publish(msg1);

	}
	else if (MessageID == 170)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64MultiArray msg1;
		msg1.layout.dim.push_back(std_msgs::MultiArrayDimension());
                msg1.layout.dim[0].size = 4;
                msg1.layout.dim[0].stride = 1;
                msg1.layout.dim[0].label = "WHEEL_SPEED_FR/WHEEL_SPEED_FL/WHEEL_SPEED_RR/WHEEL_SPEED_RL";
                
                msg1.data.clear();
		msg1.data.push_back(data.var1);	//WHEEL_SPEED_FR
		msg1.data.push_back(data.var2);	//WHEEL_SPEED_FL
		msg1.data.push_back(data.var3);	//WHEEL_SPEED_RR
		msg1.data.push_back(data.var4);	//WHEEL_SPEED_RL
		car_state_wheel_speeds_pub.publish(msg1);

	}
	else if (MessageID == 180)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //SPEED
		_car_state_vel_x_pub.publish(msg1);

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
		car_radar_track_a0_pub.publish(msg1);

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
		car_radar_track_a1_pub.publish(msg1);

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
		car_radar_track_a2_pub.publish(msg1);

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
		car_radar_track_a3_pub.publish(msg1);

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
		car_radar_track_a4_pub.publish(msg1);

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
		car_radar_track_a5_pub.publish(msg1);

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
		car_radar_track_a6_pub.publish(msg1);

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
		car_radar_track_a7_pub.publish(msg1);

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
		car_radar_track_a8_pub.publish(msg1);

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
		car_radar_track_a9_pub.publish(msg1);

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
		car_radar_track_a10_pub.publish(msg1);

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
		car_radar_track_a11_pub.publish(msg1);

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
		car_radar_track_a12_pub.publish(msg1);

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
		car_radar_track_a13_pub.publish(msg1);

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
		car_radar_track_a14_pub.publish(msg1);

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
		car_radar_track_a15_pub.publish(msg1);

	}
	else if (MessageID == 467)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::Point msg1;
		msg1.x = data.var1; //MAIN_ON
		msg1.y = data.var2; //SET_SPEED
		pcm_cruise_2_pub.publish(msg1);

		std_msgs::Int16 msg2;
		msg2.data = data.var2; //SET_SPEED
		acc_set_speed_pub.publish(msg2);

	}
	else if (MessageID == 552)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64MultiArray msg1;
		msg1.layout.dim.push_back(std_msgs::MultiArrayDimension());
                msg1.layout.dim[0].size = 1;
                msg1.layout.dim[0].stride = 1;
                msg1.layout.dim[0].label = "ACCEL_X";
                
                msg1.data.clear();
		msg1.data.push_back(data.var1);	//ACCEL_X
		car_state_imu_x_pub.publish(msg1);

	}
	else if (MessageID == 608)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //STEER_TORQUE_DRIVER
		steer_torque_driver_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //STEER_TORQUE_EPS
		steer_torque_eps_pub.publish(msg2);

		std_msgs::Int16 msg3;
		msg3.data = data.var3; //STEER_OVERRIDE
		steer_override_pub.publish(msg3);

	}
	else if (MessageID == 740)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Int16 msg1;
		msg1.data = data.var1; //STEER_REQUEST
		steer_request_pub.publish(msg1);

		std_msgs::Float64 msg2;
		msg2.data = data.var2; //STEER_TORQUE_CMD
		steer_torque_cmd_pub.publish(msg2);

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
		msg3.data = data.var2; //MINI_CAR
		acc_mini_car_pub.publish(msg3);

		std_msgs::Int16 msg4;
		msg4.data = data.var5; //ACC_CUT_IN
		acc_acc_cut_in_pub.publish(msg4);

		std_msgs::Int16 msg5;
		msg5.data = data.var6; //ACC_MALFUNCTION
		acc_acc_malfunction_pub.publish(msg5);

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

		std_msgs::Int16 msg3;
		msg3.data = data.var3; //SET_BTN
		acc_set_btn_pub.publish(msg3);

		std_msgs::Int16 msg4;
		msg4.data = data.var4; //RES_BTN
		acc_res_btn_pub.publish(msg4);

		std_msgs::Int16 msg5;
		msg5.data = data.var5; //CANCEL_BTN
		acc_cancel_btn_pub.publish(msg5);

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
		acc_set_distance_pub.publish(msg2);

		std_msgs::String msg3;
		msg3.data = data.choice_var3; //CRUISE_CONTROL_STATE
		acc_cruise_state_pub.publish(msg3);

		std_msgs::Int16 msg4;
		msg4.data = data.var3; //CRUISE_CONTROL_STATE
		acc_cruise_state_int_pub.publish(msg4);

		std_msgs::Float64 msg5;
		msg5.data = data.var2; //UI_SET_SPEED
		acc_set_speed2_pub.publish(msg5);

	}
	else if (MessageID == 956)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Bool msg1;
		msg1.data = data.var1; //SPORT_ON
		car_state_sport_mode_pub.publish(msg1);

		std_msgs::Bool msg2;
		msg2.data = data.var2; //ECON_ON
		car_state_eco_mode_pub.publish(msg2);

	}
	else if (MessageID == 1570)
	{
		data = obj.decode_message (MessageID, Message);
		std_msgs::Float64 msg1;
		msg1.data = data.var1; //HIGH_BEAMS_ON
		highbeams_pub.publish(msg1);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher car_state_imu_x_y_yaw_pub;
	ros::Publisher car_state_steering_pub;
	ros::Publisher car_state_wheel_speeds_pub;
	ros::Publisher _car_state_vel_x_pub;
	ros::Publisher car_radar_track_a0_pub;
	ros::Publisher car_radar_track_a1_pub;
	ros::Publisher car_radar_track_a2_pub;
	ros::Publisher car_radar_track_a3_pub;
	ros::Publisher car_radar_track_a4_pub;
	ros::Publisher car_radar_track_a5_pub;
	ros::Publisher car_radar_track_a6_pub;
	ros::Publisher car_radar_track_a7_pub;
	ros::Publisher car_radar_track_a8_pub;
	ros::Publisher car_radar_track_a9_pub;
	ros::Publisher car_radar_track_a10_pub;
	ros::Publisher car_radar_track_a11_pub;
	ros::Publisher car_radar_track_a12_pub;
	ros::Publisher car_radar_track_a13_pub;
	ros::Publisher car_radar_track_a14_pub;
	ros::Publisher car_radar_track_a15_pub;
	ros::Publisher pcm_cruise_2_pub;
	ros::Publisher acc_set_speed_pub;
	ros::Publisher car_state_imu_x_pub;
	ros::Publisher steer_torque_driver_pub;
	ros::Publisher steer_torque_eps_pub;
	ros::Publisher steer_override_pub;
	ros::Publisher steer_request_pub;
	ros::Publisher steer_torque_cmd_pub;
	ros::Publisher acc_accel_cmd_pub;
	ros::Publisher acc_acc_info_pub;
	ros::Publisher acc_mini_car_pub;
	ros::Publisher acc_acc_cut_in_pub;
	ros::Publisher acc_acc_malfunction_pub;
	ros::Publisher lead_dist_pub;
	ros::Publisher rel_vel_pub;
	ros::Publisher acc_set_btn_pub;
	ros::Publisher acc_res_btn_pub;
	ros::Publisher acc_cancel_btn_pub;
	ros::Publisher cruise_state_pub;
	ros::Publisher acc_set_distance_pub;
	ros::Publisher acc_cruise_state_pub;
	ros::Publisher acc_cruise_state_int_pub;
	ros::Publisher acc_set_speed2_pub;
	ros::Publisher car_state_sport_mode_pub;
	ros::Publisher car_state_eco_mode_pub;
	ros::Publisher highbeams_pub;

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