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
	side_radar_track1_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track1",1000);
	side_radar_track2_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track2",1000);
	side_radar_track3_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track3",1000);
	side_radar_track4_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track4",1000);
	side_radar_track5_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track5",1000);
	side_radar_track6_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track6",1000);
	side_radar_track8_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track8",1000);
	side_radar_track9_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track9",1000);
	side_radar_track10_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track10",1000);
	side_radar_track11_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track11",1000);
	side_radar_track12_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track12",1000);
	side_radar_track13_pub = n_.advertise<geometry_msgs::PointStamped>("side_radar_track13",1000);
	sonar1A_pub = n_.advertise<geometry_msgs::PointStamped>("sonar1A",1000);
	sonar1B_pub = n_.advertise<geometry_msgs::PointStamped>("sonar1B",1000);
	sonar2A_pub = n_.advertise<geometry_msgs::PointStamped>("sonar2A",1000);
	sonar2B_pub = n_.advertise<geometry_msgs::PointStamped>("sonar2B",1000);
	sonar3A_pub = n_.advertise<geometry_msgs::PointStamped>("sonar3A",1000);
	sonar3B_pub = n_.advertise<geometry_msgs::PointStamped>("sonar3B",1000);
	sonar3C_pub = n_.advertise<geometry_msgs::PointStamped>("sonar3C",1000);

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
	else if (MessageID == 423)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
                msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LEAD_DIST1
                msg1.point.y = data.var2; //BUTTON_PRESS0
                msg1.point.z = data.var3; //BUTTON_PRESS1
		side_radar_track1_pub.publish(msg1);

		geometry_msgs::PointStamped msg2;
		msg2.header.frame_id = "front_laser_link";
                msg2.header.stamp = ros::Time(std::stod(Time));
		msg2.point.x = data.var4; //LEAD_DIST2
		side_radar_track2_pub.publish(msg2);

		geometry_msgs::PointStamped msg3;
		msg3.header.frame_id = "front_laser_link";
                msg3.header.stamp = ros::Time(std::stod(Time));
		msg3.point.x = data.var7; //LEAD_DIST3
		side_radar_track3_pub.publish(msg3);

		geometry_msgs::PointStamped msg4;
		msg4.header.frame_id = "front_laser_link";
                msg4.header.stamp = ros::Time(std::stod(Time));
		msg4.point.x = data.var10; //LEAD_DIST4
		side_radar_track4_pub.publish(msg4);

		geometry_msgs::PointStamped msg5;
		msg5.header.frame_id = "front_laser_link";
                msg5.header.stamp = ros::Time(std::stod(Time));
		msg5.point.x = data.var13; //LEAD_DIST5
		side_radar_track5_pub.publish(msg5);

		geometry_msgs::PointStamped msg6;
		msg6.header.frame_id = "front_laser_link";
                msg6.header.stamp = ros::Time(std::stod(Time));
		msg6.point.x = data.var16; //LEAD_DIST6
		side_radar_track6_pub.publish(msg6);

	}
	else if (MessageID == 425)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
                msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //LEAD_DIST1
                msg1.point.y = data.var2; //BUTTON_PRESS0
                msg1.point.z = data.var3; //BUTTON_PRESS1
		side_radar_track8_pub.publish(msg1);

		geometry_msgs::PointStamped msg2;
		msg2.header.frame_id = "front_laser_link";
                msg2.header.stamp = ros::Time(std::stod(Time));
		msg2.point.x = data.var4; //LEAD_DIST2
		side_radar_track9_pub.publish(msg2);

		geometry_msgs::PointStamped msg3;
		msg3.header.frame_id = "front_laser_link";
                msg3.header.stamp = ros::Time(std::stod(Time));
		msg3.point.x = data.var7; //LEAD_DIST3
		side_radar_track10_pub.publish(msg3);

		geometry_msgs::PointStamped msg4;
		msg4.header.frame_id = "front_laser_link";
                msg4.header.stamp = ros::Time(std::stod(Time));
		msg4.point.x = data.var10; //LEAD_DIST4
		side_radar_track11_pub.publish(msg4);

		geometry_msgs::PointStamped msg5;
		msg5.header.frame_id = "front_laser_link";
                msg5.header.stamp = ros::Time(std::stod(Time));
		msg5.point.x = data.var13; //LEAD_DIST5
		side_radar_track12_pub.publish(msg5);

		geometry_msgs::PointStamped msg6;
		msg6.header.frame_id = "front_laser_link";
                msg6.header.stamp = ros::Time(std::stod(Time));
		msg6.point.x = data.var16; //LEAD_DIST6
		side_radar_track13_pub.publish(msg6);

	}
	else if (MessageID == 771)
	{
		data = obj.decode_message (MessageID, Message);
		geometry_msgs::PointStamped msg1;
		msg1.header.frame_id = "front_laser_link";
                msg1.header.stamp = ros::Time(std::stod(Time));
		msg1.point.x = data.var1; //SONAR1
                msg1.point.y = data.var2; //SONAR2
                msg1.point.z = data.var3; //SONAR3
		sonar1A_pub.publish(msg1);

		geometry_msgs::PointStamped msg2;
		msg2.header.frame_id = "front_laser_link";
                msg2.header.stamp = ros::Time(std::stod(Time));
		msg2.point.x = data.var4; //SONAR4
                msg2.point.y = data.var5; //SONAR5
                msg2.point.z = data.var6; //SONAR6
		sonar1B_pub.publish(msg2);

		geometry_msgs::PointStamped msg3;
		msg3.header.frame_id = "front_laser_link";
                msg3.header.stamp = ros::Time(std::stod(Time));
		msg3.point.x = data.var7; //SONAR7
                msg3.point.y = data.var8; //SONAR8
                msg3.point.z = data.var9; //SONAR9
		sonar2A_pub.publish(msg3);

		geometry_msgs::PointStamped msg4;
		msg4.header.frame_id = "front_laser_link";
                msg4.header.stamp = ros::Time(std::stod(Time));
		msg4.point.x = data.var10; //SONAR10
                msg4.point.y = data.var11; //SONAR11
                msg4.point.z = data.var12; //SONAR12
		sonar2B_pub.publish(msg4);

		geometry_msgs::PointStamped msg5;
		msg5.header.frame_id = "front_laser_link";
                msg5.header.stamp = ros::Time(std::stod(Time));
		msg5.point.x = data.var13; //SONAR13
                msg5.point.y = data.var14; //SONAR14
                msg5.point.z = data.var15; //SONAR15
		sonar3A_pub.publish(msg5);

		geometry_msgs::PointStamped msg6;
		msg6.header.frame_id = "front_laser_link";
                msg6.header.stamp = ros::Time(std::stod(Time));
		msg6.point.x = data.var16; //SONAR16
                msg6.point.y = data.var17; //SONAR17
                msg6.point.z = data.var18; //SONAR18
		sonar3B_pub.publish(msg6);

		geometry_msgs::PointStamped msg7;
		msg7.header.frame_id = "front_laser_link";
                msg7.header.stamp = ros::Time(std::stod(Time));
		msg7.point.x = data.var19; //SONAR19
            msg7.point.y = data.var20; //SONAR20
		sonar3C_pub.publish(msg7);

	}
}
private:
	ros::NodeHandle n_;
	ros::Publisher steering_angle_pub;
	ros::Publisher vel_pub;
	ros::Publisher side_radar_track1_pub;
	ros::Publisher side_radar_track2_pub;
	ros::Publisher side_radar_track3_pub;
	ros::Publisher side_radar_track4_pub;
	ros::Publisher side_radar_track5_pub;
	ros::Publisher side_radar_track6_pub;
	ros::Publisher side_radar_track8_pub;
	ros::Publisher side_radar_track9_pub;
	ros::Publisher side_radar_track10_pub;
	ros::Publisher side_radar_track11_pub;
	ros::Publisher side_radar_track12_pub;
	ros::Publisher side_radar_track13_pub;
	ros::Publisher sonar1A_pub;
	ros::Publisher sonar1B_pub;
	ros::Publisher sonar2A_pub;
	ros::Publisher sonar2B_pub;
	ros::Publisher sonar3A_pub;
	ros::Publisher sonar3B_pub;
	ros::Publisher sonar3C_pub;

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