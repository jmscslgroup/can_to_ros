#!/bin/bash

DESCRIPTION=$1

AWAKE=$(rostopic echo -n 1 /car/panda/gps_active)
echo "rosbag_record is now alive!"

#Get the vin number of the vehicle
VIN=$(cat /etc/libpanda.d/vin)

DATE=$(date +%Y_%m_%d)

TIME=$(date +%Y_%m_%d_%H_%M_%S)

FILENAME="${TIME}_${VIN}"

# If true is supplied then we are running HWIL on PI
if [[ $2 == 'true' ]]
then
	DIR_PATH="/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"
fi

# if no second argument is supplied, it means we are runnong HWIL on PI 
if [[ -z "$2" ]]
then
	DIR_PATH="/var/panda/CyverseData/JmscslgroupData/bagfiles/${DATE}/"
fi

if [[ $2 == "false" ]]
then
        hs=`echo $USER`
	DIR_PATH="/home/${hs}/.ros/${DATE}/"
fi

mkdir -p ${DIR_PATH}

rosbag record -O ${DIR_PATH}${FILENAME}${DESCRIPTION} -a -x /realtime_raw_data
