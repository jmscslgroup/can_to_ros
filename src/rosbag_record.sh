#!/bin/bash

DESCRIPTION=$1

#Get the vin number of the vehicle
VIN=$(cat /etc/libpanda.d/vin)

DATE=$(date +%Y_%m_%d)

TIME=$(date +%Y_%m_%d_%H_%M_%S)

FILENAME="${TIME}_${VIN}"

DIR_PATH="/home/safwan/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"

mkdir -p ${DIR_PATH}

rosbag record -O ${DIR_PATH}${FILENAME}${DESCRIPTION} -a -x /realtime_raw_data