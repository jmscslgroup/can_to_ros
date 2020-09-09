#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped
import datetime as dt

def speed_callback(data):
    timestamps=data.header.stamp.to_sec()
    time_h=dt.datetime.fromtimestamp(timestamps).strftime('%Y-%m-%d %H:%M:%S')
    print("Time ", time_h)
    # time_h=dt.datetime.fromtimestamp(timestamps)

    # pass
def leaddist_callback(data):
    # print("Lead dist ", data.point.x)
    # rospy.set_param('Lead_dist', data.point.x)
    # getting the parameter 
    # global_name = rospy.get_param("/global_name")
    pass
def listener():

    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("/vehicle/vel", TwistStamped, speed_callback)
    rospy.Subscriber("/vehicle/distanceEstimator/dist", PointStamped, leaddist_callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
