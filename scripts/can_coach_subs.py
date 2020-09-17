#!/usr/bin/env python
import sys
import binascii
import time
import datetime
import csv
import numpy as np
import liveRadar as lt
import kalmanTracking as kt
import random
import threading

import rospy
from std_msgs.msg import String
from geometry_msgs.msg import TwistStamped
from geometry_msgs.msg import PointStamped
from std_msgs.msg import Float64
from visualization_msgs.msg import Marker
from geometry_msgs.msg import TwistStamped

import threading, time, random
from playsound import playsound

#initializeing kalman filter object
df = 0.04
leader = kt.KF_Object([5,0],0, 0, df, 1,1,1)
#initializing ROS value variables
gnewLeadMeasurement = 1
gmyDetections = []
gnewVel=0.0
#initializing CAN Coach variables
velocity = 40
th = 3.14159
relv = 0
lead = 0
#initializing sound feedback variables
mode = "vmatch"
thSet = 2
thBounds = 0.05

#testing that sound works as things start up
try:
	fast = '/home/eternity/catkin_ws/src/can_to_ros/fastSound.wav' #path from typical installation
	playsound(fast)
except:
	print('you need to change the absolute path to the sound files in "can_coach_subs.py"')
slow = '/home/eternity/catkin_ws/src/can_to_ros/slowSound.wav' #path from typical installation 

def printit():
	"""This is the function that gives the sound feedback to the driver. It need to know the 'mode', or test that the driver is in.
	It subscribes to both the mode, and keeps track of the current mode."""
		
	t = threading.Timer(0.5, printit)
	t.start()
<<<<<<< HEAD
	global relv
=======
	print(th, lead, relv)
	rospy.set_param('relv', relv)
>>>>>>> b2ba9f6dc16efe05534c7567fec08a724088dd48
	if mode == 'cth' or mode == 'dth': #this is used for tests 1 and 2
		print(th, lead, relv)
		if th > thSet+thBounds:
			rospy.loginfo("faster")
			playsound(fast)
		if th < thSet-thBounds and th != -1:
			playsound(slow)
			rospy.loginfo("slower")
	if mode == 'vmatch': #this is used for test 3
		print(relv)
		if relv > 0 + 0.4: #0.2 m/s is less than 0.5 mph
			rospy.loginfo("faster vmatch")
			playsound(fast)
		if relv < 0 - 0.4: #0.2 m/s is less than 0.5 mph
			rospy.loginfo("slower vmatch")
			playsound(slow)
	if mode == 'ghost': #this is used for test 4
		if ghostTh > thSet+thBounds:
			rospy.loginfo("faster")
			playsound(fast)
		if ghostTh < thSet-thBounds and ghostTh != -1:
			playsound(slow)
			rospy.loginfo("slower")

def callback869(data):
    global gnewLeadMeasurement
    gnewLeadMeasurement = data.point.x

def callbackvel(data):
    global gnewVel 
    gnewVel= data.twist.linear.x

def callback384(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
    
def callback385(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
    
def callback386(data):
	global gmyDetections
	gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback387(data):
	global gmyDetections
	gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback388(data):
	global gmyDetections
	gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback389(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback390(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback391(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback392(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback393(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback394(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
    
def callback395(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
 
def callback396(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
  
def callback397(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])
    
def callback398(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

def callback399(data):
    global gmyDetections
    gmyDetections.append([data.pose.position.x, data.pose.position.y,data.pose.position.z])

#subscribe to the mode message
	#0 is standby -- no can coach feedback
	#1 is cth
	#2 is dth
	#3 is vmatch
	#4 is ghost car
		#wherever bagfile starts, publish ghostTh

#subscribe to th set point message -- thSet
	#1 is 1.35
	#2 is 1.8
	#3 is 2.25

 
try:
	rospy.init_node('can_coach_subs', anonymous=True)
	rospy.Subscriber("/vehicle/vel", TwistStamped,callbackvel)
	print('subscribing')
	rospy.Subscriber("/vehicle/distanceEstimator/dist", PointStamped, callback869)
	rospy.Subscriber("/track_a0", Marker, callback384)
	rospy.Subscriber("/track_a1", Marker, callback385)
	rospy.Subscriber("/track_a2", Marker, callback386)
	rospy.Subscriber("/track_a3", Marker, callback387)
	rospy.Subscriber("/track_a4", Marker, callback388)
	rospy.Subscriber("/track_a5", Marker, callback389)
	rospy.Subscriber("/track_a6", Marker, callback390)
	rospy.Subscriber("/track_a7", Marker, callback391)
	rospy.Subscriber("/track_a8", Marker, callback392)
	rospy.Subscriber("/track_a9", Marker, callback393)
	rospy.Subscriber("/track_a10", Marker, callback394)
	rospy.Subscriber("/track_a11", Marker, callback395)
	rospy.Subscriber("/track_a12", Marker, callback396)
	rospy.Subscriber("/track_a13", Marker, callback397)
	rospy.Subscriber("/track_a14", Marker, callback398)
	rospy.Subscriber("/track_a15", Marker, callback399)

	r = rospy.Rate(50)

	printit()
	while not rospy.is_shutdown():
		print('starting while loop')
		velocity = gnewVel
		while not rospy.is_shutdown():

			if velocity > 0:
				th = (leader.get_coords()[0])/(velocity) #distance divided by meters per second
			else:
				th = -1
			leader.predict()
			if gnewLeadMeasurement != None:#if there's a new 869 lead measurement
				#cluster 869 and radar points and choose one radar point
				if len(gmyDetections) > 0: # if there are new radar measurements
					labels = lt.clusterRadar([gnewLeadMeasurement,0],gmyDetections) #labels are points within 1m euclidean distance
					myPoints = labels 
					
					if len(myPoints) > 0: #if there is a point other than from 869 in cluster
						iLead = random.choice(myPoints)#just choose one randomly
						lead = gmyDetections[iLead][0:2] #lead coordinates
						relv = gmyDetections[iLead][2]
						gnewLeadMeasurement = None
						gmyDetections = []#reset the radar message buffer
						leader.update(lead) #update kf
			else:
				if len(gmyDetections) > 64: # if there are 64 radar measurements and no gnewLeadMeasurement
					labels = lt.clusterRadar(leader.get_coords().tolist(),gmyDetections) #cluster algorithm gives labels
					myPoints = labels
					
					if len(myPoints) > 0: #if there is a point other than from kalman value in cluster
						iLead = random.choice(myPoints)#just choose one randomly
						lead = gmyDetections[iLead][0:2] #lead coordinates
						relv = gmyDetections[iLead][2] #lead relv
						
						gmyDetections = []#reset the radar message buffer
						leader.update(lead) #update kf
        #this feedback system operates under the assumption that there is a lead vehicle
			velocity = gnewVel
                
		print('before sleep')
		# t.cancel()
		r.sleep()
	print('before spin')
	# t.cancel()
except ValueError:
    print("Oops!  Try again...")
