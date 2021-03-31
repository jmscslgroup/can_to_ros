#!/usr/bin/env python


import rospy
from std_msgs.msg import Float64
import sys

class Selector:
    def __init__(self):
        rospy.init_node('selector', anonymous=True)
        self.pub = rospy.Publisher('accel_cmd', Float64, queue_size=10)
        self.rate = rospy.Rate(100)

    def publisher(self):
        #print ("Enter (1) for + acceleration (2) for - acceleration: ")
        #val = input("") 
        if len(sys.argv) < 2:
            print("Pleas pass accel value")
            sys.exit()
        val = sys.argv[1]

        while not rospy.is_shutdown():
            msg = Float64()
            msg.data = float(val)
            # rospy.loginfo(hello_str)
            self.pub.publish(msg)
            self.rate.sleep()

if __name__ == '__main__':
    try:
        obj1 = Selector()
        obj1.publisher()

    except:
        print("An exception occurred")