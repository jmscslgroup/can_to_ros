# can_to_ros
Read CAN messges from csv file in ROS
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
$ cd ~/carkin_ws/src/can_to_ros
$ git checkout conver_msg #change branch
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
$roslaunch can_to_ros publish_can_msg.launch
```
[![Plotting RPM message from Rav4 CAN bus](http://img.youtube.com/vi/cmY9Eaxb0kc/0.jpg)](https://www.youtube.com/watch?v=cmY9Eaxb0kc)




