#!/usr/bin/env python


import rospy
from std_msgs.msg import Float64
import sys
import subprocess
import time

class NodeStatus:
    def __init__(self):
        rospy.init_node('NodeStatus', anonymous=True)
        self.pub = rospy.Publisher('/timed_accel', Float64, queue_size=10)
        self.rate = rospy.Rate(10)

    def publisher(self):

        while not rospy.is_shutdown():
            try:
                output = subprocess.check_output("rosnode list", shell=True).decode('utf-8')
                if 'ghost_mode_node' in output:
                    # print('pid is running')
                    pass  # pid node is running
                else:
                    msg = Float64()
                    msg.data = 0.0
                    # rospy.loginfo(hello_str)
                    self.pub.publish(msg)
                    self.rate.sleep()

            except:
                print("node_status except")

if __name__ == '__main__':
    try:
        obj1 = NodeStatus()
        obj1.publisher()

    except:
        print("An exception occurred")