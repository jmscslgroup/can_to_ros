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



class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    //Topic you want to publish
    accel_pub = n_.advertise<std_msgs::Float64>("accel", 1000);  
    lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist", 1000);
    str_angle_pub = n_.advertise<std_msgs::Float64>("steering_angle", 1000);
    speed_pub = n_.advertise<geometry_msgs::Twist>("vel", 1000);
    tracka0_pub = n_.advertise<geometry_msgs::PointStamped>("track_a0", 1000);
    tracka1_pub = n_.advertise<geometry_msgs::PointStamped>("track_a1", 1000);
    tracka2_pub = n_.advertise<geometry_msgs::PointStamped>("track_a2", 1000);
    tracka3_pub = n_.advertise<geometry_msgs::PointStamped>("track_a3", 1000);
    tracka4_pub = n_.advertise<geometry_msgs::PointStamped>("track_a4", 1000);
    tracka5_pub = n_.advertise<geometry_msgs::PointStamped>("track_a5", 1000);
    tracka6_pub = n_.advertise<geometry_msgs::PointStamped>("track_a6", 1000);
    tracka7_pub = n_.advertise<geometry_msgs::PointStamped>("track_a7", 1000);
    tracka8_pub = n_.advertise<geometry_msgs::PointStamped>("track_a8", 1000);
    tracka9_pub = n_.advertise<geometry_msgs::PointStamped>("track_a9", 1000);
    tracka10_pub = n_.advertise<geometry_msgs::PointStamped>("track_a10", 1000);
    tracka11_pub = n_.advertise<geometry_msgs::PointStamped>("track_a11", 1000);
    tracka12_pub = n_.advertise<geometry_msgs::PointStamped>("track_a12", 1000);
    tracka13_pub = n_.advertise<geometry_msgs::PointStamped>("track_a13", 1000);
    tracka14_pub = n_.advertise<geometry_msgs::PointStamped>("track_a14", 1000);
    tracka15_pub = n_.advertise<geometry_msgs::PointStamped>("track_a15", 1000);
    // trackb0_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b0", 1000);
    // trackb1_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b1", 1000);
    // trackb2_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b2", 1000);
    // trackb3_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b3", 1000);
    // trackb4_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b4", 1000);
    // trackb5_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b5", 1000);
    // trackb6_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b6", 1000);
    // trackb7_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b7", 1000);
    // trackb8_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b8", 1000);
    // trackb9_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b9", 1000);
    // trackb10_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b10", 1000);
    // trackb11_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b11", 1000);
    // trackb12_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b12", 1000);
    // trackb13_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b13", 1000);
    // trackb14_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b14", 1000);
    // trackb15_pub = n_.advertise<geometry_msgs::PointStamped>("/track_b15", 1000);

    // headlights_pub = n_.advertise<can_to_ros::headlights>("/highbeams", 1000);
    // headlights_pub = n_.advertise< std_msgs::UInt8>("/highbeams", 1000);
    
    headlights_pub = n_.advertise< std_msgs::Float64 >("highbeams", 1000);


    //Topic you want to subscribe
    sub_ = n_.subscribe("/realtime_raw_data", 1000, &SubscribeAndPublish::callback, this);
  }

  void callback(const std_msgs::String::ConstPtr& raw_data)
  {
    
    std::stringstream ss(raw_data->data);
    
    ss >> Time>> Bus>> MessageID>> Message>> MessageLength;

    // std::cout << MessageID << std::endl;
    if(MessageID == 869)
    {
     data = obj.decode_message (MessageID, Message);
     std_msgs::Float64 dist;
    //  std::cout << data.var1 << std::endl;
     dist.data = data.var1;
        //      std::time_t epoch1 = stod(Time);// creating a time_tobject to store time
        // struct tm * ptm=std::localtime(&epoch1); // gmtime converts time since epoch to calendar time expressed as Universal Coordinated Time
        // // struct tm is structure holding a calendar date and time broken down into its components.
        // int hour= ptm->tm_hour;
        // int min= ptm->tm_min;
        // int sec= ptm->tm_sec;
        // std::cout << hour<<":"<<min<< ":"<< sec<< std::endl;
      // if ( data.var1 < 252){
     lead_dist_pub.publish(dist);
      // }
    }

    else if (MessageID == 1570 && Bus ==0)
    {
    data = obj.decode_message (MessageID, Message);
    std_msgs::Float64 msg;
    // std_msgs::UInt8 msg;

    msg.data=data.var2;

    // can_to_ros::headlights msg;
    // msg.timestamp=ros::Time(std::stod(Time));
    // msg.light_state_changed=data.var1;
    // msg.high_beams_on=data.var2;
    // msg.head_lamps_on=data.var3;
    // msg.running_lights_on=data.var4;
    headlights_pub.publish(msg);
    }

    else if (MessageID == 552)
    { 
      data = obj.decode_message (MessageID, Message);
      std_msgs::Float64  msg;
      
      msg.data = data.var1;
      accel_pub.publish(msg);
      
    }
    else if (MessageID == 37 && Bus == 0)
    { 
       data = obj.decode_message (MessageID, Message);
       std_msgs::Float64  str_angle;
   
       str_angle.data = data.var1;
       str_angle_pub.publish(str_angle);

    }
    else if (MessageID == 180 && Bus == 0)
    { 
      // std::cout << "speed msg" << std::endl;
       data = obj.decode_message (MessageID, Message); 
       geometry_msgs::Twist msg;
       msg.linear.x = data.var1;
       speed_pub.publish(msg);
    }
    else if (MessageID == 384)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka0_pub.publish(marker);
    }
    else if (MessageID == 385)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed




      tracka1_pub.publish(marker);
    }
    else if (MessageID == 386)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka2_pub.publish(marker);
    }
    else if (MessageID == 387)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka3_pub.publish(marker);
    }
    else if (MessageID == 388)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
  
      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka4_pub.publish(marker);
    }
    else if (MessageID == 389)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka5_pub.publish(marker);
    }
    else if (MessageID == 390)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed
 

      tracka6_pub.publish(marker);
    }
    else if (MessageID == 391)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed

      tracka7_pub.publish(marker);
    }
    else if (MessageID == 392)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed
  

      tracka8_pub.publish(marker);
    }
    else if (MessageID == 393)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed
  
      tracka9_pub.publish(marker);
    }
    else if (MessageID == 394)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed

      tracka10_pub.publish(marker);
    }
        else if (MessageID == 395)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed

 

      tracka11_pub.publish(marker);
    }
        else if (MessageID == 396)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka12_pub.publish(marker);
    }
        else if (MessageID == 397)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));
 
      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


      tracka13_pub.publish(marker);
    }
        else if (MessageID == 398)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed
  

      tracka14_pub.publish(marker);
    }
        else if (MessageID == 399)
    { 

      data = obj.decode_message (MessageID, Message);
      geometry_msgs::PointStamped marker;
      marker.header.frame_id = "front_laser_link";
      marker.header.stamp = ros::Time(std::stod(Time));

      marker.point.x = data.var2; // 
      marker.point.y = (data.var1 * -1.0); // 
      marker.point.z = data.var3; // rel speed


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
  // ros::Publisher trackb0_pub;
  // ros::Publisher trackb1_pub;
  // ros::Publisher trackb2_pub;
  // ros::Publisher trackb3_pub;
  // ros::Publisher trackb4_pub;
  // ros::Publisher trackb5_pub;
  // ros::Publisher trackb6_pub;
  // ros::Publisher trackb7_pub;
  // ros::Publisher trackb8_pub;
  // ros::Publisher trackb9_pub;
  // ros::Publisher trackb10_pub;
  // ros::Publisher trackb11_pub;
  // ros::Publisher trackb12_pub;
  // ros::Publisher trackb13_pub;
  // ros::Publisher trackb14_pub;
  // ros::Publisher trackb15_pub;
  ros::Publisher headlights_pub;

  ros::Subscriber sub_;
  decode_msgs obj;
  std::string Time,Buffer,Message,MessageLength;
  double MessageID, Bus;
  values data;

};//End of class SubscribeAndPublish

// }
/****************************************************/
int main(int argc, char **argv){
    ros::init(argc, argv, "subs_fs");
    ros::NodeHandle nh1;
    // ROS_INFO("Got parameter");

    SubscribeAndPublish SAPObject;
     ros::spin();   

  return 0;
}
