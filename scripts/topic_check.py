#!/usr/bin/env python


import rospy
from std_msgs.msg import Float64
from std_msgs.msg import Bool
import sys
import subprocess
import threading # Needed for Timer
import time

class TopicCheck:
    def __init__(self):
        rospy.init_node('TopicCheck', anonymous=True)
        # self.pub = rospy.Publisher('/', Float64, queue_size=10)
        rospy.Subscriber("/car/libpanda/controls_allowed", Bool, self.callback)
        self.timer = threading.Timer(5,self.timeout) # If n seconds elapse, call timeout()
        self.timer.start()
        rospy.spin()
        # self.rate = rospy.Rate(10)


    def timeout(self):
        print("No message received for 5 seconds")
        subprocess.check_output("echo -1 > /etc/libpanda.d/controls_allowed", shell=True)
        # Do something

    def callback(self, msg):
        # global timer
        print("Message received")
        self.timer.cancel()
        self.timer = threading.Timer(5,self.timeout)
        self.timer.start()

        output = subprocess.check_output("rostopic echo -n1 /car/libpanda/controls_allowed", shell=True).decode('utf-8')
        var1 = output.replace('data: ', '')
        var1 = var1.replace('---', '')
        # print(var1)
        data = var1.strip()

        # while("" in data) : 
        #     data.remove("")

        print(data)

        if (data == "True" ):
            subprocess.check_output("echo 1 > /etc/libpanda.d/controls_allowed", shell=True)
        
        elif (data == "False"):
            subprocess.check_output("echo 0 > /etc/libpanda.d/controls_allowed", shell=True)
        # Do Other thing

    # def publisher(self):

    #     while not rospy.is_shutdown():
    #         try:
    #             output = subprocess.check_output("rostopic echo -n1 /car/libpanda/controls_allowed", shell=True).decode('utf-8')
    #             # output = subprocess.check_output("rostopic echo -n1 /test", shell=True).decode('utf-8')
            
    #             # print(output)
    #             var1 = output.replace('data: ', '')
    #             var1 = var1.replace('---', '')
    #             # print(var1)
    #             data = var1.strip()

    #             # while("" in data) : 
    #             #     data.remove("")

    #             print(data)

    #             if not data:
    #                 subprocess.check_output("echo -1 > /etc/libpanda.d/controls_allowed", shell=True)

    #             elif (data == "True" ):
    #                 subprocess.check_output("echo 1 > /etc/libpanda.d/controls_allowed", shell=True)
                
    #             elif (data == "False"):
    #                 subprocess.check_output("echo 0 > /etc/libpanda.d/controls_allowed", shell=True)


    #             # if any(node in output for node in node_list):
    #             #     # print('pid is running')
    #             #     # print("Working")
    #             #     pass  # pid node is running
    #             # else:
    #             #     # print("something")
    #             #     msg = Float64()
    #             #     msg.data = 0.0
    #             #     # rospy.loginfo(hello_str)
    #             #     self.pub.publish(msg)
    #             #     self.pub_1.publish(msg)
    #             #     self.rate.sleep()

    #         except:
    #             print("topic_check except")

if __name__ == '__main__':
    try:
        obj1 = TopicCheck()
        # obj1.publisher()

    except Exception as e:
        print (e)
        print("An exception occurred")