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
