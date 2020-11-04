# can_to_ros (README needs to be updated)
can_to_ros is a ROS package that reads CAN bus messages from provided csv file or in real time, decode the messages, publish them to ROS topics, and record the published messages.
### Prerequisites

ROS melodic

## Example
1.Create a ROS Workspace
```
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
```
2.Clone the repos.

**Installing libpanda**
```
$ cd ~/catkin_ws/
$ git clone https://github.com/jmscslgroup/libpanda.git  
$ #Follow the installation guide on https://github.com/jmscslgroup/libpanda
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

Use roslaunch to start publishing:
<pre>
$roslaunch can_to_ros piping.launch bag_name:= name_of_the_bag_file.

This launch file will create file inside the home directory named <b>can_data.csv</b>, and then it will pipe the data written to that file to a ros node that handles publishing raw data. 

The generated bag file can be found inside the home directory.
</pre>
