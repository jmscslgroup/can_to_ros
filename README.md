# can_to_ros
can_to_ros is a ROS package has the following purposes:
* Reads CAN bus messages from provided csv file or in real time [(Through libpanda library)](https://github.com/jmscslgroup/libpanda), decode the messages, publish them to ROS topics, and record the published messages.
* Implementing CAN coach system. (We can talk more about that)
### Prerequisites

Ubuntu 18.04
ROS melodic

## Installation
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
$ #Follow the installation guide [here](https://github.com/jmscslgroup/libpanda)
```
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
$source ./devel/setup.bash
```
**Publishing from a CSV file**

Use roslaunch to start publishing:
```
$roslaunch can_to_ros start_can_decoding.launch can_path:=/path/to/file.csv bag_name:= name_of_the_bag_file.

The generated bag file can be found inside the home directory.
```
**Publishing in real time (Needs to be updated)**

