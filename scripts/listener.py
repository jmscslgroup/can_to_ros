#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped

def speed_callback(data):
    #  print("Speed ", data.twist.linear.x)
    pass
def leaddist_callback(data):
    # print("Lead dist ", data.point.x)
    # rospy.set_param('Lead_dist', data.point.x)
    # getting the parameter 
    # global_name = rospy.get_param("/global_name")
    pass
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("/vehicle/vel", TwistStamped, speed_callback)
    rospy.Subscriber("/vehicle/distanceEstimator/dist", PointStamped, leaddist_callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
