#!/usr/bin/env python

# This is a minimal example that works with can_speed_decoder
# This subscribes to a message named "/vehicle/vel", of datatype geometry_msgs/Twist
# If a car is not handy, then you can test this with speed_talker.py

import rospy

# Here we import the ROS message types
from geometry_msgs.msg import Twist

def speedCallback(data):
    # Do stuff with data (in this case print to the screen):
    rospy.loginfo(rospy.get_caller_id() + 'Speed: %s', data.linear.x)

def hello_speed():
    # Initialize node with name:
    rospy.init_node('hello_speed', anonymous=True)

    # Invoke the subscriber with callback:
    rospy.Subscriber('vehicle/vel', Twist, speedCallback)

    # Run indefinitely:
    rospy.spin()

if __name__ == '__main__':
    hello_speed()
