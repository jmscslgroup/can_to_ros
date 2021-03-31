# can_to_ros

can_to_ros is a ROS package that provide a real-time and offline decoding of CAN bus messages.


### Software Requirements
-------------------------
* Ubuntu 18.04 or 20.04
* ROS [melodic](http://wiki.ros.org/melodic/Installation/Ubuntu) or ROS [noetic](http://wiki.ros.org/noetic/Installation)
* [libpanda](https://github.com/jmscslgroup/libpanda) (must be installed in /opt directory)

## Installation
---------------

1.Create a ROS Workspace
```
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
```
2.Clone the repos.

**Installing libpanda**
```
$ cd /opt
$ git clone https://github.com/jmscslgroup/libpanda.git  
```
Follow the installation guide [here](https://github.com/jmscslgroup/libpanda)

**Cloning the ROS package**
```
$ cd ~/catkin_ws/src/
$ git clone https://github.com/SafwanElmadani/can_to_ros.git
```
3.Build the WS
```
$ cd ~/catkin_ws
$ catkin_make
```
4.Source your enviroment:
```
$source ~/catkin_ws/devel/setup.bash
```

