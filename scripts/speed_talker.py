#!/usr/bin/env python

#  This is a simple example that demonstrates how to publish a message
#  to ROS with type "geometry_msgs/Twist"
#  This is intended to be test code to send messages to hello_speed.py

import rospy
from geometry_msgs.msg import Twist

def speed_talker():

    # This generates the topic "vehicle/vel" with type "geometry_msgs/Twist"
    pub = rospy.Publisher('vehicle/vel', Twist, queue_size=10)

    # Let's roscore know that this is a node, with name "speed_talker"
    rospy.init_node('speed_talker', anonymous=True)
    rate = rospy.Rate(2) # 2hz

    speed = 0
    speed_msg = Twist()  # initialize the ROS message

    while not rospy.is_shutdown():
        rospy.loginfo('New Speed: %s', speed)  # Print to the screen
	speed_msg.linear.x = speed	# populate the message's data
        pub.publish(speed_msg)		# publish the message
        rate.sleep()	# gives us the above 2Hz rate
        speed = speed + 1

if __name__ == '__main__':
    try:
        speed_talker()
    except rospy.ROSInterruptException:
        pass
