#!/bin/bash

#Get the vin number of the vehicle
VIN=$(cat /etc/libpanda.d/vin)

DATE=$(date +%Y_%m_%d)

FILENAME="${DATE}_${VIN}"

DIR_PATH="/home/safwan/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"

mkdir -p ${DIR_PATH}

rosbag record -O ${DIR_PATH}${FILENAME} -a -x /realtime_raw_data