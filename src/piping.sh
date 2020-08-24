#!/bin/bash

touch ~/can_data.csv
tail -fn 1 ~/can_data.csv | rosrun can_to_ros realtime_publishing