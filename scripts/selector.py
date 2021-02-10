#!/usr/bin/env python


import rospy
from std_msgs.msg import Int8
import time

class Selector:
    def __init__(self):
        rospy.init_node('selector', anonymous=True)
        self.pub = rospy.Publisher('choice', Int8, queue_size=10)
        self.rate = rospy.Rate(100)

    def publisher(self):
        print ("Enter (1) for + acceleration (2) for - acceleration: ")
        val = input("") 

        while not rospy.is_shutdown():
            msg = Int8()
            msg.data = int(val)
            # rospy.loginfo(hello_str)
            self.pub.publish(msg)
            self.rate.sleep()

if __name__ == '__main__':
    try:
        obj1 = Selector()
        obj1.publisher()

    except:
        print("An exception occurred")