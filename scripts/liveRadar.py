#from panda import Panda
import binascii
#import bitstring
import time
import datetime
import csv
#import cantools
#import pandas as pd
import numpy as np
#import matplotlib.pyplot as plt
import random
from sklearn.cluster import AgglomerativeClustering
import math

#Author: Matthew Nice
#Contact: matthew.nice@vanderbilt.edu


def database(filename):

    db = cantools.database.Database()
    with open (filename, 'r') as fin:
        db.add_dbc_string(fin.read())
    return db

#db2 = database('RAV4.dbc')

def connectPanda():
    try:
        p = Panda() #create panda object
    except:
        print('could not connect to Panda')
        p = 0

    return p

def getRadar(can_recv, addRelv = False): 
    """Returns the information from the radar sensor if available in can frame.
    Set addRelv to True if you want the relv value too."""
    RADAR = [385,386,387,388,389,390,391,392,393,394,395,396,397,398,399]
    # db2 = database('RAV4.dbc')
    lat = 0
    lon = 0
    relv = 0
    for addr, _, msg, src  in can_recv:
        if addr in RADAR: #get data and timestamp of radar measurement
            #if that decoded radar message fits all of the .lon .lat
            lat = list(db2.decode_message(addr,msg).values())[2] #latitude
            lon = list(db2.decode_message(addr,msg).values())[1] #longitude
            relv = list(db2.decode_message(addr,msg).values())[4] #rel velocity
            #add getting relv somewhere
            if lon < 327:
                # print(relv)
                if addRelv == False:
                    return lat,lon
                if addRelv == True:
                    # print('you have relv')
                    return lat,lon,relv

class myDetections():

    def __init__(self,initialD = np.array([[0,0]])):
        self.detections = initialD

    def storeDetections(self, detection):
        self.detections = np.concatenate(self.detections,detection)

        return self.detections

    #request accumulated detections
    def getDetections():
        return self.detections

    #reset detections after they are fed into the kalman filter
    def resetDetections():
        self.detections = np.array([[0,0]])

def getLeadDist(can_recv):
    distance = 0
    # db2 = database('RAV4.dbc')
    for addr, _, msg, src  in can_recv:
        if addr ==  869:
            distance = list(db2.decode_message(addr,msg).values())[6]
            if distance < 252:
                return distance

def clusterRadar(referencePosition, radar_batch):
    ##cluster = AgglomerativeClustering(n_clusters=2, affinity='euclidean', linkage='ward')
    #cluster = AgglomerativeClustering(n_clusters=None, distance_threshold = 1, affinity='euclidean', linkage='ward')
    # Dx2 observations in the radar_batch, returns a cluster label for each point
    #return cluster.fit_predict(radar_batch)
	myPoints = []

	for i in range (len(radar_batch)):
		dist = math.sqrt( ((referencePosition[0]-radar_batch[i][0])**2 )+ ((referencePosition[1]-radar_batch[i][1])**2) )
		if dist < 1:
			myPoints.append(i)
	
	return myPoints
	

def dateString(posixTime):
    timeObj = time.localtime(posixTime)
    date = timeObj.tm_mon, timeObj.tm_mday, timeObj.tm_year, timeObj.tm_hour, timeObj.tm_min, timeObj.tm_sec
    d =  '%d-%d-%d_%d-%d-%d' % (date)

    return date,d

def getClusteredRadar( writer = None,recentLeadMeasurement = None, velNaccel = False, gnewLeadMeasurement = None, gmyDetections = None):
    """This function returns a radar sensor measurement from the leading object.
    It returns the position coordinates and the relative velocity.

    If there is a .04 second timeout, it will return None.

    writer is where incoming data is written. recentLeadMeasurement can be used as a
     feedforward of a recent output measurement so you don't have to wait for
     a new 869 messge to come in"""


    end = False
    velocity = None
    accel = None
    RADAR = [385,386,387,388,389,390,391,392,393,394,395,396,397,398,399]
    myDetections = gmyDetections
    # np.empty_like([[0,0]]) #set myDetections
    myRelv = np.array([0])
    tstart = time.time()

    if recentLeadMeasurement != None:
        leadMeasurement =  recentLeadMeasurement
    else:
        leadMeasurement = None

    while (end == False) & (tstart + .045 > time.time()): #one second timeout
        # print('loop')
        # can_recv = pandaObject.can_recv()
        if gnewLeadMeasurement != None:
            newLeadMeasurement = gnewLeadMeasurement
            # print(newLeadMeasurement)

        radar = None
        if writer != None:
            for address, _, dat, src  in can_recv:
                # currTime = time.time()
                # writer.writerow(([str(currTime), str(src), str((address)), len(dat), str(binascii.hexlify(dat).decode('utf-8'))]))
                # # if address ==  869:
                # #     distance = list(db2.decode_message(address,dat).values())[6]
                # #     if distance < 252:
                # #         newLeadMeasurement = distance
                # #         print(distance)
                # if velNaccel == True:
                #     if address == 552:
                #         # print(dat)
                #         dat = bytes(dat).hex().ljust(16,'0')
                #         accel = list(db2.decode_message(address,bytes.fromhex(dat)).values())[0]
                #     if address == 180:
                #         velocity = list(db2.decode_message(address,dat).values())[1]


                if address in RADAR: #get data and timestamp of radar measurement
                    #if that decoded radar message fits all of the .lon .lat
                    lat = list(db2.decode_message(address,dat).values())[2] #latitude
                    lon = list(db2.decode_message(address,dat).values())[1] #longitude
                    relv = list(db2.decode_message(address,dat).values())[4] #rel velocity
                    #add getting relv somewhere
                    if lon < 327:
                        radar = [lat,lon,relv]
        # radar = getRadar(can_recv,addRelv=True) #lat, lon coordinates, relv from radar sensor
        # newLeadMeasurement = getLeadDist(can_recv) #869 measurement. Reliable true positive for lead object.
        # print(radar,newLeadMeasurement)
        if radar != None: #if there is a new radar measurement
            coords = radar[0],radar[1]
            # print(coords)
            relv = radar[2]
            myDetections = np.concatenate((myDetections,[coords]),0)#add new detection
            myRelv = np.concatenate((myRelv,[relv]),0) #add new detection

        if newLeadMeasurement != None: #if there is a new lead measurement
            leadMeasurement = newLeadMeasurement #update lead measurement
        # print(myDetections)
        #cluster 869 and radar points and choose one radar point
        if len(myDetections) > 1: #and leadMeasurement != None: # if there are new radar measurements
            print("inside if")
            radarPlus = np.concatenate((myDetections,[[0,leadMeasurement]]) )#radar measurements plus 869 measurement
            if len(radarPlus) > 2:
                labels = clusterRadar(radarPlus) #cluster algorithm gives labels
            i869 = labels[-1] #the cluster the 869 measurement is in
            indices = [i for i, x in enumerate(labels) if x == i869]#find relevant indices of good cluster points
            #indices = np.where(labels == i869)[0] #this should be faster

            myPoints = indices[:-1] #take out 869 from the key points
            # print(myPoints,indices)
            if len(myPoints) > 0: #if there is a point other than from 869 in cluster
                iLead = random.choice(myPoints)#just choose one randomly
                lead = myDetections[iLead] #lead coordinates
                relv = myRelv[iLead] #lead relv
                print ("lead", lead)
                end = True #we have lead vehicle points, so stop running the loop
                # print(lead,relv)

    if end == True:
        if velNaccel == True:
            return lead,relv,velocity,accel
        else:
            return lead,relv
    else:
        #if there is a timeout and no value to report
        # print('radar measurement timeout')
        if velNaccel == True:
            return None,None,velocity,accel
        else:
            return None,None

def getTurnSignal(can_recv):
    turnSignal = ''
    TURN_SIGNALS = 1556
    # db2 = database('RAV4.dbc')
    for addr, _, msg, src  in can_recv:
        if addr == TURN_SIGNALS: #get data and timestamp of radar measurement
            #if that decoded radar message fits all of the .lon .lat
            turnSignal = list(db2.decode_message(addr,msg).values())[0] #turn signal
            return turnSignal

def getBrake(can_recv):
    brake = ''
    # db2 = database('RAV4.dbc')

    for addr, _, msg, src  in can_recv:
        if addr == 166:
            brake = list(db2.decode_message(addr,msg).values())[1]
            return brake

def getGear(can_recv):
    GEAR = 956
    gear = ''
    for addr, _, msg, src  in can_recv:
        if addr == GEAR:
            gear = list(db2.decode_message(addr,msg).values())[0]
            return gear

def getSteering(can_recv):
    STEERING = 37
    steer = 0
    for addr, _, msg, src  in can_recv:
        if addr == STEERING:
            steer = list(db2.decode_message(addr,msg).values())[0]
            return steer

def getVelocity(can_recv):
    VELOCITY = 180
    v = 0
    for addr, _, msg, src  in can_recv:
        if addr == VELOCITY:
            v = list(db2.decode_message(addr,msg).values())[1]
            return v
def liveRadar(p,depth, width,lat100,lon100,fig,ax):
    """Plot the radar points as they come in, in the XY-plane. Turn signal is used to clear data from axes.
    Turn on the left turn signal to clear axes and reset--this avoids time lag.

    depth: the limit of the depth for your live radar field.
    width: the limit of the width for your live radar field. absolute value of width. i.e. +/- width.
    p: Panda Object
    """
    can_recv = p.can_recv()

    if getRadar(can_recv) != None:
        lat,lon = getRadar(can_recv)
    if lat != None and abs(lat) < width and lon < depth:
        updateLast100(lat100,lon100,lat,lon)
        plotLiveRadar(fig,ax,depth,width,lat100,lon100)
