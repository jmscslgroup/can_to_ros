#!/bin/bash

#Get the vin number of the vehicle
VIN=$(cat /etc/libpanda.d/vin)

DATE=$(date +%Y_%m_%d)

TIME=$(date +%Y_%m_%d_%k_%M_%S)

FILENAME="${TIME}_${VIN}"

DIR_PATH="/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"

mkdir -p ${DIR_PATH}

rosbag record -O ${DIR_PATH}${FILENAME} -a -x /realtime_raw_data