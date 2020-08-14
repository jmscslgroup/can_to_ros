from __future__ import division
from scipy.optimize import linear_sum_assignment
import numpy as np
import matplotlib.pyplot as plt
from filterpy.kalman import KalmanFilter
import time

#Author: Derek Gloudemans, with modifications by Matthew Nice
#Contact: matthew.nice@vanderbilt.edu

def match_hungarian(first,second,iou_cutoff = 0.5):
    """
    performs  optimal (in terms of sum distance) matching of points
    in first to second using the Hungarian algorithm
    inputs - N x 2 arrays of object x and y coordinates from different frames
    output - M x 1 array where index i corresponds to the second frame object
    matched to the first frame object i
    """
    # find distances between first and second
    dist = np.zeros([len(first),len(second)])
    # print(first,second)
    for i in range(0,len(first)):
        for j in range(0,len(second)):
            dist[i,j] = np.sqrt((first[i,0]-second[j,0])**2 + (first[i,1]-second[j,1])**2)

    a, b = linear_sum_assignment(dist)

    # convert into expected form
    matchings = np.zeros(len(first))-1
    for idx in range(0,len(a)):
        matchings[a[idx]] = b[idx]
    matchings = np.ndarray.astype(matchings,int)

    return matchings


class KF_Object():
    """
    A wrapper class that stores a Kalman filter for tracking the object as well
    as some other parameters, variables and all object positions
    """
    def __init__(self, xy,id_num, delta_t, frame_num,mod_err,meas_err,state_err):
        # use mod_err,meas_err, and state_err to tune filter

        self.first_frame = frame_num # first frame in which object is detected
        self.fsld = 0 # frames since last detected
        self.all = [] # all positions of object across frames
        self.tags = []
        self.id = id_num

        self.is_lead_object = False

        t = delta_t

        # intialize state (generally x but called state to avoid confusion here)
        state = np.zeros([4,1])
        state[0,0] = xy[0]
        state[1,0] = xy[1]

        # state transition matrix
        F = np.identity(4)
        for i in range(0,2):
            F[i,i+2] = t

        # initialize measurement transition matrix
        H = np.zeros([2,4])
        H[0,0] = 1
        H[1,1] = 1
        #add here relv measurements
        # H[0,2] = 1
        # print(state_err)

        # initialize Kalman Filter to track object
        self.kf = KalmanFilter(dim_x = 4, dim_z = 2)
        self.kf.x = state # state
        self.kf.P *= state_err # state error covariance matrix
        self.kf.Q = np.identity(4)*mod_err # model error covariance matrix
        self.kf.R = np.identity(2)* meas_err # measurement error covariance matrix
        self.kf.F = F
        self.kf.H = H


    def predict(self):
        self.kf.predict()

    def update(self,measurement):
        self.kf.update(measurement)

    def get_x(self):
        """
        returns current state, so will return a priori state estimate if
        called after predict, or a posteriori estimate if called after update
        """
        return self.kf.x

    def get_coords(self):
        """
        returns 1d numpy array of x,y,s,r
        """
        return self.kf.x[[0,1],0]


class KF_Tracker():
    def __init__(self,delta_t,mod_err=1,meas_err=1,state_err=1,fsld_max = 10):
        """
        Initializes tracker
        delta_t - (float) - time in seconds between detections
        mod_err - model error for kalman filter
        meas_err - measurement error for kalman filter
        state_err - state error for kalman filter (starting val)
        fsld_max - maximum number of frames in which an object can go undetected
                        before it is considered lost
        """
        self.active_objs = []
        self.inactive_objs = []

        self.mod_err = mod_err
        self.meas_err = meas_err
        self.state_err = state_err
        self.fsld_max = fsld_max
        self.id_counter = 0
        self.frame_num = 0
        self.delta_t = delta_t

        self.has_lead_object = False


    def __call__(self,detections):
        """
        Updates tracked object states, creating and destroying KF_objects as
        necessary
        detections - [n x 2] Numpy array of xy coordinates for all detected objects
        returns - dictionary of xy coords (1x2 numpy) keyed by object ids
        """

        # 1. predict new locations of all objects x_k | x_k-1
        for obj in self.active_objs:
            obj.predict()

        # 2. look at next set of detected objects - all objects are included in this even if detached
        # convert into numpy array - where row i corresponds to object i in active_objs
        locations = np.zeros([len(self.active_objs),2])
        for i,obj in enumerate(self.active_objs):
            locations[i,:] = obj.get_coords()

        # 3. match - these arrays are both N x 2
        # remove matches with IOU below threshold (i.e. too far apart)
        matches = match_hungarian(locations,detections)

        # traverse object list
        move_to_inactive = []
        for i in range(0,len(self.active_objs)):
            obj = self.active_objs[i]

            # update fsld and delete if too high
            if matches[i] == -1:
                obj.fsld += 1
                obj.all.append(obj.get_coords())
                obj.tags.append(0) # indicates object not detected in this frame
                if obj.fsld > self.fsld_max:
                    move_to_inactive.append(i)

            # update Kalman filter
            else: # object was matched
                measure_coords = detections[matches[i]]
                obj.update(measure_coords)
                obj.fsld = 0
                obj.all.append(obj.get_coords())
                obj.tags.append(1) # indicates object detected in this frame

        # for all unmatched objects, intialize new object
        for j in range(0,len(detections)):
            if j not in matches:
                new_obj = KF_Object(detections[j],
                                    self.id_counter,
                                    self.frame_num,
                                    self.delta_t,
                                    self.mod_err,
                                    self.meas_err,
                                    self.state_err)
                new_obj.all.append(new_obj.get_coords())
                new_obj.tags.append(1) # indicates object detected in this frame
                self.active_objs.append(new_obj)
                self.id_counter += 1

        # move all necessary objects to inactive list
        move_to_inactive.sort()
        move_to_inactive.reverse()
        for idx in move_to_inactive:
            self.inactive_objs.append(self.active_objs[idx])
            del self.active_objs[idx]


        self.frame_num += 1

        active_object_locations = {}
        for obj in self.active_objs:
            active_object_locations[obj.id] = obj.get_coords()

        #4. check for active lead object
        #objects will be designated lead if/when CAN ID 869 and match_lead() are used
        self.check_for_lead()

        return active_object_locations

    def all_objs(self):
        """returns all active and inactive objects as two lists"""
        return self.active_objs,self.inactive_objs

    #look at active objects and find the closest match
    #this function assumes that if a measurement comes from 869 there exists a lead object in the radar data
    def match_lead(self, lead_dist):
        #match the measurement to an active object:
        detection = np.array([[0, lead_dist]])#measurement from 869
        active_object_locations = []
        for obj in self.active_objs:
            active_object_locations.append(obj.get_coords())
        active_object_locations = np.array(active_object_locations)
        # print('Active: ',active_object_locations, 'D:',detection)
        match = match_hungarian(detection,active_object_locations)

        for o in self.active_objs:#set the matched object to the lead object
            # print(o.get_coords(), active_object_locations[match])
            if np.all(o.get_coords() == active_object_locations[match][0]):#if the coordinates of the object are the match
                o.is_lead_object = True #assign the object to being the lead
                lead = o
            else:
                o.is_lead_object = False # no other object is lead object
        if len(self.active_objs)>0:
            return lead
        else:
            # print("No active objects",end='\r')
            return None

    def check_for_lead(self): #check if there is an active lead object
        self.has_lead_object = False
        for o in self.active_objs:
            if o.is_lead_object == True:
                self.has_lead_object = True

        return self.has_lead_object
