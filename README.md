# can_to_ros
can_to_ros is a ROS package that reads CAN bus messages and GPS data from provided csv files, decode the messages, and publish them to ROS topics.
### Prerequisites

ROS melodic

## Example
1.Create a ROS Workspace
```
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
```
2.Clone the repo.
```
$ cd ~/catkin_ws/src
$ git clone https://github.com/SafwanElmadani/can_to_ros.git
```
3.Build the WS
```
$ cd ~/catkin_ws
$ catkin_make
```
4.Source your enviroment:
```
$source ./devel/setup.bash
```
Then use roslaunch to start can_msg_decoder node:
```
$roslaunch can_to_ros start_can_decoding.launch can_path:=/path/to/file.csv gps_path:=/path/to/file.csv bag_name:= name of the bag file.
```
Change the bag file path in start_can_decoding.launch
```
currently, the path is /home/safwan/Documents/test_ws/$(arg bag_name)_CAN_GPS
needs to be changed to a path on your system. /my_path/$(arg bag_name)_CAN_GPS
```



