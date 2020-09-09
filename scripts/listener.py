#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped
#include "visualization_msgs/Marker.h"
from visualization_msgs.msg import Marker
import datetime as dt

def speed_callback(data):
    timestamps=data.header.stamp.to_sec()
    # time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    # print("Time ", time_h)
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
    timestamps=data.header.stamp.to_sec()
    time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    print("Time ", time_h)
    print("z0: ", data.pose.position.z)
    #np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0) 
    # print ("trackA0")
    # print(data.pose.position.x, data.pose.position.y)
# def callback385(data):
#     print("z1 ", data.pose.position.z)    # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0) 
# def callback386(data):
#     print("z2: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0) 
# def callback387(data):
#     print("z3: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback388(data):
#     print("z4: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback389(data):
#     print("z5: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback390(data):
#     print("z6: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback391(data):
#     print("z7: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback392(data):
#     print("z8: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback393(data):
#     print("z9: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback394(data):
#     print("z10: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback395(data):
#     print("z11: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback396(data):
#     print("z12: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback397(data):
#     print("z13: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback398(data):
#     print("z14: ", data.pose.position.z)
#     # np.append(gmyDetections,[[data.pose.position.x, data.pose.position.y]],axis=0)
# def callback399(data):
#     print("z15: ", data.pose.position.z)
def listener():

    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("/vehicle/vel", TwistStamped, speed_callback)
    rospy.Subscriber("/vehicle/distanceEstimator/dist", PointStamped, leaddist_callback)
    rospy.Subscriber("/track_a0", Marker, callback384)
    # rospy.Subscriber("/track_a1", Marker, callback385)
    # rospy.Subscriber("/track_a2", Marker, callback386)
    # rospy.Subscriber("/track_a3", Marker, callback387)
    # rospy.Subscriber("/track_a4", Marker, callback388)
    # rospy.Subscriber("/track_a5", Marker, callback389)
    # rospy.Subscriber("/track_a6", Marker, callback390)
    # rospy.Subscriber("/track_a7", Marker, callback391)
    # rospy.Subscriber("/track_a8", Marker, callback392)
    # rospy.Subscriber("/track_a9", Marker, callback393)
    # rospy.Subscriber("/track_a10", Marker, callback394)
    # rospy.Subscriber("/track_a11", Marker, callback395)
    # rospy.Subscriber("/track_a12", Marker, callback396)
    # rospy.Subscriber("/track_a13", Marker, callback397)
    # rospy.Subscriber("/track_a14", Marker, callback398)
    # rospy.Subscriber("/track_a15", Marker, callback399)


    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
