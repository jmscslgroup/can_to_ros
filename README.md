# can_to_ros
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
**Publishing from a CSV file**
Use roslaunch to start publishing:
```
$roslaunch can_to_ros start_can_decoding.launch can_path:=/path/to/file.csv bag_name:= name_of_the_bag_file.

The generated bag file can be found inside the home directory.
```
**Publishing in real time**
Use roslaunch to start publishing:
```
$roslaunch can_to_ros piping.launch bag_name:= name_of_the_bag_file.

This launch file will create file inside the home directory named *can_data.csv*, and then it will pipe the data written to that file to a ros node that handles publishing rawe data. 

The generated bag file can be found inside the home directory.
```



