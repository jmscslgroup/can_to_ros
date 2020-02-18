# CANSpeedDecoder
Decode CAN messges written in csv file.

### Prerequisites

ROS melodic

## Example
First source your enviroment:
```
$source ./devel/setup.bash
```
Then use roslaunch to start can_msg_decoder node:
```
roslaunch <pakage name> publish_can_msg.launch file_path:=/home/<USER>/.../CAN_Message_.csv
```



