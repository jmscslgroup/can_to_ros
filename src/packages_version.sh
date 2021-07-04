#!/bin/bash

#Get the vin number of the vehicle

source ~/.bashrc

FILEPATH1=$(rospack find ghost_mode)

FILEPATH2=$(rospack find transfer_pkg)

FILEPATH3=$(rospack find can_to_ros)

FILEPATH4=$(rospack find ghostfollower)

ghost_mode=`git -C ${FILEPATH1} checkout reference_tracking`

ghost_mode=`git -C ${FILEPATH1} branch --show-current`
transfer_pkg=`git -C ${FILEPATH2} branch --show-current`
can_to_ros=`git -C ${FILEPATH3} branch --show-current`
# ghostfollower=`git -C ${FILEPATH4} branch --show-current`


printf "ghost_mode branch: ${ghost_mode}\ntransfer_pkg branch: ${transfer_pkg}\ncan_to_ros branch: ${can_to_ros}\nghostfollower branch: ${ghostfollower}\n"


# VIN=$(cat /etc/libpanda.d/vin)

# DATE=$(date +%Y_%m_%d)

# TIME=$(date +%Y_%m_%d_%H_%M_%S)

# FILENAME="${TIME}_${VIN}"

# DIR_PATH="/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"

# mkdir -p ${DIR_PATH}

# rosbag record -O ${DIR_PATH}${FILENAME} -a -x /realtime_raw_data

# reference_tracking