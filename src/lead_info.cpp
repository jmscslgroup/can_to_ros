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
//#include "can_to_ros/headlights.h"
#include "std_msgs/UInt8.h"
#include <cstdlib>



class LeadInfo
{
	public:

		double r_lat = 0.0;
		double r_long = 0.0;
		double r_velocity = 0.0;
		double lead_distance = 0.0;
		double last_read_lead_dist = 0.0;
		double old_ego_velocity = 0.0;
		double current_ego_velocity =0.0; 
		double old_r_velocity = 0.0;

		std::vector <geometry_msgs::PointStamped> radar_traces;
		LeadInfo()
		{
			radar_traces = std::vector<geometry_msgs::PointStamped>(16);
			//Topic you want to publish
			relative_vel_pub = n_.advertise<geometry_msgs::Twist>("rel_vel", 1);
			lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist", 1);
			leaddist_sub = n_.subscribe("lead_dist_869", 1, &LeadInfo::callback_lead, this);
			ego_vel_sub = n_.subscribe("vel", 1, &LeadInfo::callback_ego_speed, this);

			//Topic you want to subscribe
			tracka0_sub = n_.subscribe("track_a0", 1, &LeadInfo::cb0, this);
			tracka1_sub = n_.subscribe("track_a1", 1, &LeadInfo::cb1, this);
			tracka2_sub = n_.subscribe("track_a2", 1, &LeadInfo::cb2, this);
			tracka3_sub = n_.subscribe("track_a3", 1, &LeadInfo::cb3, this);
			tracka4_sub = n_.subscribe("track_a4", 1, &LeadInfo::cb4, this);
			tracka5_sub = n_.subscribe("track_a5", 1, &LeadInfo::cb5, this);
			tracka6_sub = n_.subscribe("track_a6", 1, &LeadInfo::cb6, this);
			tracka7_sub = n_.subscribe("track_a7", 1, &LeadInfo::cb7, this);
			tracka8_sub = n_.subscribe("track_a8", 1, &LeadInfo::cb8, this);
			tracka9_sub = n_.subscribe("track_a9", 1, &LeadInfo::cb9, this);
			tracka10_sub = n_.subscribe("track_a10", 1, &LeadInfo::cb10, this);
			tracka11_sub = n_.subscribe("track_a11", 1, &LeadInfo::cb11, this);
			tracka12_sub = n_.subscribe("track_a12", 1, &LeadInfo::cb12, this);
			tracka13_sub = n_.subscribe("track_a13", 1, &LeadInfo::cb13, this);
			tracka14_sub = n_.subscribe("track_a14", 1, &LeadInfo::cb14, this);
			tracka15_sub = n_.subscribe("track_a15", 1, &LeadInfo::cb15, this);
		}


		void cb0(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(0) = p;
			
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}

		}
		void cb1(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(1) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
		}
		void cb2(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(2) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
		}
		void cb3(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(3) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb4(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(4) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb5(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(5) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb6(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(6) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb7(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(7) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb8(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(8) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb9(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(9) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb10(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(10) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb11(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(11) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb12(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(12) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb13(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(13) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb14(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(14) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
		void cb15(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			
			geometry_msgs::PointStamped p;
			p.point.x = radar->point.x;
			p.point.y = radar->point.y;
			p.point.z = radar->point.z;
			radar_traces.at(15) = p;
			std::vector <int> index_vector;
			for(int k=0; k < radar_traces.size(); ++k)
			{
				if(abs(radar_traces.at(k).point.y) <=0.5)
				{
					index_vector.push_back(k);
				}
			}
			if (index_vector.size() > 0 )
			{

				double min_dist = radar_traces.at( index_vector.at(0)).point.x;
				double desired_index = 0;
				for(int m = 1; m < index_vector.size() ; ++ m)
				{
					if( radar_traces.at( index_vector.at(m) ).point.x  < min_dist )
					{
						min_dist = radar_traces.at( index_vector.at(m) ).point.x ;
						desired_index = m;
					}
				}
				old_r_velocity = r_velocity;

				r_velocity = radar_traces.at( index_vector.at(desired_index) ).point.z;
				double lat_of_min_dist = radar_traces.at( index_vector.at(desired_index) ).point.y;
				if (min_dist > 250.0)
				{
					min_dist = 250.0;
					r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/

				}
				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = min_dist; //long
				msg.linear.y = lat_of_min_dist; //lat
				msg.linear.z = r_velocity; // rel_v
				relative_vel_pub.publish(msg);
				dist.data = min_dist;
				lead_dist_pub.publish(dist);
			}
			
		}
# if 0
		void callback(const geometry_msgs::PointStamped::ConstPtr& radar)
		{
			// std::cout << "long: " << radar->point.x << std::endl; 
			// std::cout << "lat: " << radar->point.y << std::endl; 
			// 20+10=30                15                20-10=10         15 
			if ( lead_distance +1 >= radar->point.x && lead_distance -1 <= radar->point.x && radar->point.x < 252)  {
				if (abs(radar->point.y) <= 0.5){
					last_read_lead_dist = radar->point.x;
					r_lat = radar->point.y;
					r_long = radar->point.x;
					old_r_velocity = r_velocity;
					r_velocity =  radar->point.z;
					geometry_msgs::Twist msg;
					std_msgs::Float64 dist;
					msg.linear.x = radar->point.x; //long 
					msg.linear.y = radar->point.y; //lat
					msg.linear.z = radar->point.z; // rel_v

					relative_vel_pub.publish(msg);   
					dist.data = radar->point.x;
					lead_dist_pub.publish(dist);


				}
			}
			else if (last_read_lead_dist +1 >= radar->point.x && last_read_lead_dist -1 <= radar->point.x && radar->point.x < 252){

				if (abs(radar->point.y) <= 0.5){
					last_read_lead_dist = radar->point.x;
					r_lat = radar->point.y;
					r_long = radar->point.x;
					old_r_velocity = r_velocity;
					r_velocity =  radar->point.z;
					geometry_msgs::Twist msg;
					std_msgs::Float64 dist;
					msg.linear.x = radar->point.x; //long 
					msg.linear.y = radar->point.y; //lat
					msg.linear.z = radar->point.z; // rel_v
					relative_vel_pub.publish(msg);
					dist.data = radar->point.x;
					lead_dist_pub.publish(dist);

				}
			}
			else {

				geometry_msgs::Twist msg;
				std_msgs::Float64 dist;
				msg.linear.x = 0.0; //long 
				msg.linear.y = 0.0; //lat
				old_r_velocity = r_velocity;
				r_velocity = 0.0;/* r_velocity =  old_r_velocity - ( current_ego_velocity -old_ego_velocity );*/
				msg.linear.z = r_velocity;
				relative_vel_pub.publish(msg);
				dist.data = 252.0;
				lead_dist_pub.publish(dist);

			}
		}


#endif
		void callback_lead(const std_msgs::Float64::ConstPtr& dist)
		{
			lead_distance = dist->data; 
			// std::cout << "lead distance is:  " << dist->data << std::endl;  
			// std::cout << "long dist from radar: " << r_long << std::endl;

		}


		void callback_ego_speed(const geometry_msgs::Twist::ConstPtr& speed)
		{
			old_ego_velocity = current_ego_velocity;
			current_ego_velocity = speed->linear.x;
		}

	private:
		ros::NodeHandle n_;
		ros::Publisher relative_vel_pub;
		ros::Publisher lead_dist_pub;

		ros::Subscriber leaddist_sub;

		ros::Subscriber ego_vel_sub;

		ros::Subscriber tracka0_sub;
		ros::Subscriber tracka1_sub;
		ros::Subscriber tracka2_sub;
		ros::Subscriber tracka3_sub;
		ros::Subscriber tracka4_sub;
		ros::Subscriber tracka5_sub;
		ros::Subscriber tracka6_sub;
		ros::Subscriber tracka7_sub;
		ros::Subscriber tracka8_sub;
		ros::Subscriber tracka9_sub;
		ros::Subscriber tracka10_sub;
		ros::Subscriber tracka11_sub;
		ros::Subscriber tracka12_sub;
		ros::Subscriber tracka13_sub;
		ros::Subscriber tracka14_sub;
		ros::Subscriber tracka15_sub;


};//End of class 

// }
/****************************************************/
int main(int argc, char **argv){
	ros::init(argc, argv, "lead_info");
	ros::NodeHandle nh1;

	LeadInfo relative_velocity;
	ros::spin();   

	return 0;
}
