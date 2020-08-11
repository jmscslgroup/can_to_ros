#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float32.h"
#include "sensor_msgs/TimeReference.h"
#include "header_package/can_decode.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/AccelStamped.h"
#include "geometry_msgs/TwistStamped.h"


class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    accel_pub = n_.advertise<geometry_msgs::AccelStamped>("/vehicle/accel", 1000);  
    lead_dist_pub = n_.advertise<geometry_msgs::PointStamped>("/vehicle/distanceEstimator/dist", 1000);
    str_angle_pub = n_.advertise<geometry_msgs::PointStamped>("/vehicle/steering_angle", 1000);
    speed_pub = n_.advertise<geometry_msgs::TwistStamped>("/vehicle/vel", 1000);
    tracka0_pub = n_.advertise<visualization_msgs::Marker>("/track_a0", 1000);
    tracka1_pub = n_.advertise<visualization_msgs::Marker>("/track_a1", 1000);
    tracka2_pub = n_.advertise<visualization_msgs::Marker>("/track_a2", 1000);
    tracka3_pub = n_.advertise<visualization_msgs::Marker>("/track_a3", 1000);
    tracka4_pub = n_.advertise<visualization_msgs::Marker>("/track_a4", 1000);
    tracka5_pub = n_.advertise<visualization_msgs::Marker>("/track_a5", 1000);
    tracka6_pub = n_.advertise<visualization_msgs::Marker>("/track_a6", 1000);
    tracka7_pub = n_.advertise<visualization_msgs::Marker>("/track_a7", 1000);
    tracka8_pub = n_.advertise<visualization_msgs::Marker>("/track_a8", 1000);
    tracka9_pub = n_.advertise<visualization_msgs::Marker>("/track_a9", 1000);
    tracka10_pub = n_.advertise<visualization_msgs::Marker>("/track_a10", 1000);
    tracka11_pub = n_.advertise<visualization_msgs::Marker>("/track_a11", 1000);
    tracka12_pub = n_.advertise<visualization_msgs::Marker>("/track_a12", 1000);
    tracka13_pub = n_.advertise<visualization_msgs::Marker>("/track_a13", 1000);
    tracka14_pub = n_.advertise<visualization_msgs::Marker>("/track_a14", 1000);
    tracka15_pub = n_.advertise<visualization_msgs::Marker>("/track_a15", 1000);


    //Topic you want to subscribe
    sub_ = n_.subscribe("/raw_data", 1000, &SubscribeAndPublish::callback, this);
  }

  void callback(const std_msgs::String::ConstPtr& raw_data)
  {
    std::stringstream ss(raw_data->data);
    ss >> Time>> Bus>> MessageID>> Message>> MessageLength;
    
    if(MessageID == 869 && Bus == 0 )
    {
     data = obj.decode_message (MessageID, Message);
     geometry_msgs::PointStamped dist;
     dist.header.stamp=ros::Time(std::stod(Time));
     dist.point.x = data.var1;
     lead_dist_pub.publish(dist);

    }
    else if (MessageID == 552 && Bus==0)
    { 
      data = obj.decode_message (MessageID, Message);
      geometry_msgs::AccelStamped msg;
      msg.header.stamp=ros::Time(std::stod(Time));
      msg.accel.linear.x = data.var1;
      accel_pub.publish(msg);
      
    }
    else if (MessageID == 37 && Bus == 0)
    { 
       data = obj.decode_message (MessageID, Message);
       geometry_msgs::PointStamped str_angle;
       str_angle.header.stamp=ros::Time(std::stod(Time));
       str_angle.point.x = data.var1;
       str_angle_pub.publish(str_angle);

    }
    else if (MessageID == 180 && Bus == 0)
    { 
       data = obj.decode_message (MessageID, Message); 
       geometry_msgs::TwistStamped msg;
       msg.header.stamp=ros::Time(std::stod(Time));
       msg.twist.linear.x = data.var1;
       speed_pub.publish(msg);
    }
    else if (MessageID == 384)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka0_pub.publish(marker);
    }
    else if (MessageID == 385)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka1_pub.publish(marker);
    }
    else if (MessageID == 386)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka2_pub.publish(marker);
    }
    else if (MessageID == 387)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka3_pub.publish(marker);
    }
    else if (MessageID == 388)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka4_pub.publish(marker);
    }
    else if (MessageID == 389)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka5_pub.publish(marker);
    }
    else if (MessageID == 390)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka6_pub.publish(marker);
    }
    else if (MessageID == 391)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka7_pub.publish(marker);
    }
    else if (MessageID == 392)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka8_pub.publish(marker);
    }
    else if (MessageID == 393)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka9_pub.publish(marker);
    }
    else if (MessageID == 394)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka10_pub.publish(marker);
    }
        else if (MessageID == 395)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka11_pub.publish(marker);
    }
        else if (MessageID == 396)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka12_pub.publish(marker);
    }
        else if (MessageID == 397)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka13_pub.publish(marker);
    }
        else if (MessageID == 398)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka14_pub.publish(marker);
    }
        else if (MessageID == 399)
    { 

      data = obj.decode_message (MessageID, Message);
      visualization_msgs::Marker marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
      marker.ns = "my_namespace";
      marker.id = 0;
      marker.type = 1;
      marker.action =0;
      marker.pose.position.x = data.var2; // 
      marker.pose.position.y = (data.var1 * -1.0); // 
      // marker.pose.position.x = 5.0;
      // marker.pose.position.y = -2.0;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w =1.0;

      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.color.a = 1.0; // Don't forget to set the alpha!
      if (data.var2 <= 3.0)
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }
      else
      {
      marker.color.r = 1.0;
      marker.color.g = 0.0;
      marker.color.b = 0.0;
      }

      // std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
      // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
      // // struct tm is structure holding a calendar date and time broken down into its components.
      // int hour= ptm->tm_hour;
      // int min= ptm->tm_min;
      // int sec= ptm->tm_sec;
      // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;

      tracka15_pub.publish(marker);
    }

}

private:
  ros::NodeHandle n_;
  ros::Publisher accel_pub;
  ros::Publisher lead_dist_pub;
  ros::Publisher str_angle_pub;
  ros::Publisher speed_pub;
  ros::Publisher tracka0_pub;
  ros::Publisher tracka1_pub;
  ros::Publisher tracka2_pub;
  ros::Publisher tracka3_pub;
  ros::Publisher tracka4_pub;
  ros::Publisher tracka5_pub;
  ros::Publisher tracka6_pub;
  ros::Publisher tracka7_pub;
  ros::Publisher tracka8_pub;
  ros::Publisher tracka9_pub;
  ros::Publisher tracka10_pub;
  ros::Publisher tracka11_pub;
  ros::Publisher tracka12_pub;
  ros::Publisher tracka13_pub;
  ros::Publisher tracka14_pub;
  ros::Publisher tracka15_pub;

  ros::Subscriber sub_;
  decode_msgs obj;
  std::string Time,Buffer,Message,MessageLength;
  int MessageID, Bus;
  values data;

};//End of class SubscribeAndPublish

// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "subs");
    ros::NodeHandle nh1;
    ROS_INFO("Got parameter");

    SubscribeAndPublish SAPObject;
     ros::spin();   

  return 0;
}