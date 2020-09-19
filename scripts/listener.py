#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped
#include "visualization_msgs/Marker.h"
from visualization_msgs.msg import Marker
import datetime as dt
import csv

# file=open("lead_speed_est.csv", 'w')
# writer=csv.writer(file)
# writer.writerow(["time","speed"])

def speed_callback(data):
    relv=rospy.get_param('/relv')
    ego_speed=data.twist.linear.x
    timestamps=data.header.stamp.to_sec()
    time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    print ("Time: ", time_h)
    print ("Relative velocity: ", relv )
    print("Ego speed: ", ego_speed)
    print("Lead velocity: ", ego_speed + relv)
    lead_velocity=ego_speed + relv
    print("*************************************************")
    # writer.writerow([timestamps,lead_velocity])
    # 2.Relative velocity: %f
    # 3.Look Up Student Record
    # 4.Exit/Quit
    # " % (time_h,relv))
    # print("Time ", time_h)
    # print ("Relative velocity ", relv )
    # time_h=dt.datetime.fromtimestamp(timestamps)

    # pass
def leaddist_callback(data):
    # timestamps=data.header.stamp.to_sec()
    # time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print("Time ", time_h)
    # print("Lead dist ", data.point.x)
    # rospy.set_param('Lead_dist', data.point.x)
    # getting the parameter 
    # global_name = rospy.get_param("/global_name")
    pass
def callback384(data):
    # timestamps=data.header.stamp.to_sec()
    # time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print("Time ", time_h)
    # print("z0: ", data.pose.position.z)
    pass


def listener():
    rospy.init_node('listener', anonymous=True)
    print(rospy.Time.now().to_sec())
    rospy.Subscriber("/vehicle/vel", TwistStamped, speed_callback)
    rospy.Subscriber("/vehicle/distanceEstimator/dist", PointStamped, leaddist_callback)
    rospy.Subscriber("/track_a0", Marker, callback384)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
