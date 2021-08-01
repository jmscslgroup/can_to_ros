#!/usr/bin/env python


import rospy
from std_msgs.msg import Float64
import sys
import subprocess
import time
import threading # Needed for Timer
from std_msgs.msg import String

class NodeStatus:
    def __init__(self):
        rospy.init_node('NodeStatus', anonymous=True)
        rospy.Subscriber("chatter", String, self.callback)
        self.rate = rospy.Rate(10)
        self.timer = threading.Timer(2,self.timeout) # If 5 seconds elapse, call timeout()
        self.timer.start()
        rospy.spin()

    def timeout(self):
        print("No message received for 2 seconds")
        # Do something

    def callback(self, msg):
        # global timer
        print("Message received")
        self.timer.cancel()
        self.timer = threading.Timer(2,self.timeout)
        self.timer.start()
        # Do Other thing


if __name__ == '__main__':
    try:
        obj1 = NodeStatus()
    except:
        print("An exception occurred")