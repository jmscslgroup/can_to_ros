#!/usr/bin/env python3
#
# Author: Rahul Bhadani
# Copyright (c) Arizona Board of Regents
# All rights reserved.

import sys
import rospy
import csv
import time
import datetime
import socket
from os.path import expanduser
import rospkg


class saveparam:
    def __init__(self, ns):
        """
        """
        pass


def main(argv):

    print("In Save param")
    rospy.init_node('saveparam', anonymous=True)
    ns = rospy.get_namespace() # Retrieve the name space
    params = rospy.get_param_names()

    param_dict = {}

    param_list = []
    val_list = []
    rospack = rospkg.RosPack()

    onnx2ros_path = rospack.get_path('onnx2ros')
    can_to_ros_path = rospack.get_path('can_to_ros')
    velocity_controller_path = rospack.get_path('velocity_controller')

    host = socket.gethostname()

    packages = ['onnx2ros', 'can_to_ros', 'velocity_controller', 'micromodel', 'margin', 'transfer_pkg']
    for pkg in packages:

        try:
            pkg_path = rospack.get_path(pkg)
            import os
            os.system("cd {} && git remote -v > /tmp/tmp.txt".format(pkg_path))
            S = open('/tmp/tmp.txt', 'r').read()
            param_list.append('{}_url'.format(pkg))
            val_list.append(S)

            os.system("cd {} && git log -n 1 > /tmp/tmp.txt".format(pkg_path))
            S = open('/tmp/tmp.txt', 'r').read()
            param_list.append('{}_commithash'.format(pkg))
            val_list.append(S)

            os.system("cd {} && git branch --show-current > /tmp/tmp.txt".format(pkg_path))
            S = open('/tmp/tmp.txt', 'r').read()
            param_list.append('{}_branch'.format(pkg))
            val_list.append(S)
        except rospkg.ResourceNotFound as e:
            print("{} package not available ... skipping.")

    import os

    libpanda_path = '/home/circles/libpanda'
    #if host in ['refulgent', 'ivory', 'starfire']:
    if not os.path.exists('/home/circles/libpanda'):
       libpanda_path = '/opt/libpanda'

    os.system("cd {} && git remote -v > /tmp/tmp.txt".format(libpanda_path))
    S = open('/tmp/tmp.txt', 'r').read()
    param_list.append('{}_url'.format('libpanda'))
    val_list.append(S)
    os.system("cd {} && git log -n 1 > /tmp/tmp.txt".format(libpanda_path))
    S = open('/tmp/tmp.txt', 'r').read()
    param_list.append('{}_commithash'.format('libpanda'))
    val_list.append(S)
    os.system("cd {} && git branch --show-current > /tmp/tmp.txt".format(libpanda_path))
    S = open('/tmp/tmp.txt', 'r').read()
    param_list.append('{}_branch'.format('libpanda'))
    val_list.append(S)


    for p in params:
        param_list.append(p)
        val = rospy.get_param(p)
        val_list.append(val)

    dt_object = datetime.datetime.fromtimestamp(time.time())
    filename  = dt_object.strftime('%Y_%m_%d_%H_%M_%S')  + '_' + argv[0] + '.csv'
    parentfolder = dt_object.strftime('%Y_%m_%d') + '/'
    host = socket.gethostname()

    home = expanduser("~")
    import os

    if argv[1] == "false":
        if not os.path.exists(home +'/.ros/' + parentfolder):
            os.makedirs(home +'/.ros/' + parentfolder)
        filename = home  +'/.ros/' + parentfolder + filename
    else:
        if not os.path.exists('/var/panda/CyverseData/JmscslgroupData/bagfiles/' + parentfolder):
            os.makedirs('/var/panda/CyverseData/JmscslgroupData/bagfiles/' + parentfolder)
        filename = '/var/panda/CyverseData/JmscslgroupData/bagfiles/' + parentfolder + filename

    print(filename)
    with open(filename, 'w') as csvfile:
        # creating a csv writer object
        csvwriter = csv.writer(csvfile)

        # writing the fields
        csvwriter.writerow(param_list)

        # writing the data rows
        csvwriter.writerow(val_list)

if __name__ == '__main__':
    main(sys.argv[1:])

