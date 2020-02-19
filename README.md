# CANSpeedDecoder
Read CAN messges from csv file, and decode msgs with id=180 to speed (m/s). It also publish the speed to ROS topic /vehicle/vel

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
$ git clone https://github.com/SafwanElmadani/CANSpeedDecoder.git
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
$roslaunch can_speed_decoder publish_can_msg.launch file_path:=/home/user/.../catkin_ws/src/CANSpeedDecoder/CAN_Message_.csv
```
The generated bag file should be located at ~/.ros directory 
```
$cd ~/.ros
```



