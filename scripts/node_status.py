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
        self.pub_1 = rospy.Publisher('/cmd_accel', Float64, queue_size=10)
        self.rate = rospy.Rate(100)

    def publisher(self):

        while not rospy.is_shutdown():
            try:
                output = subprocess.check_output("rosnode list", shell=True).decode('utf-8')
                node_list = ['ghost_mode_node', 'velocity_controller_real_vehicle_node']

                if any(node in output for node in node_list):
                    # print('pid is running')
                    pass  # pid node is running
                else:
                    msg = Float64()
                    msg.data = 0.0
                    # rospy.loginfo(hello_str)
                    self.pub.publish(msg)
                    self.pub_1.publish(msg)
                    self.rate.sleep()

            except:
                print("node_status except")

if __name__ == '__main__':
    try:
        obj1 = NodeStatus()
        obj1.publisher()

    except:
        print("An exception occurred")