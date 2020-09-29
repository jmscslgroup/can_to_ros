#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped
#include "visualization_msgs/Marker.h"
from visualization_msgs.msg import Marker
from std_msgs.msg import Float64
from std_msgs.msg import Float32
import datetime as dt
import csv
from scipy import integrate
#import pandas as pd
#import scipy
#import matplotlib.pyplot as plt
#import numpy as np


# file=open("lead_speed_est.csv", 'w')
# writer=csv.writer(file)
# writer.writerow(["time","speed"])
# glead_velocity=0.0
# pub_ghostvel = rospy.Publisher('/ghost_vehicle/vel', TwistStamped, queue_size=10)
pub_spacegap = rospy.Publisher('/space_gap', Float32, queue_size=10)
relv=0
ghost_dist=0
def speed_callback(data): # calculating the velocity of the lead vehicle 
    msg=TwistStamped()
    # relv=rospy.get_param('/relv')
    # print("relv :", relv)
    ego_speed=data.twist.linear.x
    timestamps=data.header.stamp.to_sec()
    time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print ("Time: ", time_h)
    # print ("Relative velocity: ", relv )
    # print("Ego speed: ", ego_speed)
    # print("Lead velocity: ", ego_speed + relv)
    lead_velocity=ego_speed + relv
    msg.header.stamp=data.header.stamp
    msg.twist.linear.x = lead_velocity
    # pub_ghostvel.publish(msg)
    # print("*************************************************")
    # writer.writerow([timestamps,lead_velocity])
    # 2.Relative velocity: %f
    # 3.Look Up Student Record
    # 4.Exit/Quit
    # " % (time_h,relv))
    # print("Time ", time_h)
    # print ("Relative velocity ", relv )
    # time_h=dt.datetime.fromtimestamp(timestamps)

    # pass
def egodist_callback(data):
    msg_spacegap=Float32()
    ego_dist=data.point.x
    space_gap=ghost_dist-ego_dist
    msg_spacegap.data=space_gap
    pub_spacegap.publish(msg_spacegap)
    # timestamps=data.header.stamp.to_sec()
    # time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print("Time ", time_h)
    # print("Lead dist ", data.point.x)
    # rospy.set_param('Lead_dist', data.point.x)
    # getting the parameter 
    # global_name = rospy.get_param("/global_name")
    # pass
def ghostdist_callback(data):
    global ghost_dist
    ghost_dist=data.point.x
    # timestamps=data.header.stamp.to_sec()
    # time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print("Time ", time_h)
    # print("Lead dist ", data.point.x)
    # rospy.set_param('Lead_dist', data.point.x)
    # getting the parameter 
    # global_name = rospy.get_param("/global_name")
    # pass
def relv_callback(data):
     global relv
     relv=data.data


def listener():
    rospy.init_node('listener', anonymous=True)
    print(rospy.Time.now().to_sec())
    rospy.Subscriber("/vehicle/vel", TwistStamped, speed_callback)
    rospy.Subscriber("/relv",Float64, relv_callback )
    rospy.Subscriber("/ego_dist_traveled", PointStamped, egodist_callback)
    rospy.Subscriber("/ghost_dist_traveled", PointStamped, ghostdist_callback)
    
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
