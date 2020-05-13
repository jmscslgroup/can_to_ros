#!/bin/bash

touch ~/can_data.csv
tail -fn 1 ~/can_data.csv | rosrun can_to_ros can_msg_decoder &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node1 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node2 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node3 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node4 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node5 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node6 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node7 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node8 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node9 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node10 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node11 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node12 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node13 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node14 &
tail -fn 1 ~/can_data.csv | rosrun can_to_ros node15 &
