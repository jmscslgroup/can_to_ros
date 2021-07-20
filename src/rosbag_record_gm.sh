#!/bin/bash

DESCRIPTION=$1

#Get the vin number of the vehicle
VIN=$(cat /etc/libpanda.d/vin)

DATE=$(date +%Y_%m_%d)

TIME=$(date +%Y_%m_%d_%H_%M_%S)

FILENAME="${TIME}_${VIN}"

DIR_PATH="/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"

mkdir -p ${DIR_PATH}

# FILEPATH1=$(rospack find ghost_mode)

# ghost_mode=`git -C ${FILEPATH1} rev-parse --abbrev-ref HEAD`

rosbag record -O "${DIR_PATH}${FILENAME}${DESCRIPTION}" -a -x /realtime_raw_data