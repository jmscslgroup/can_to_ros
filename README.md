# can_to_ros

can_to_ros is a ROS package that provide a real-time and offline decoding of CAN bus messages. It is also integrated with [libpanda](https://github.com/jmscslgroup/libpanda) to send control commands to Toyota RAV4.


### Software Requirements and Dependencies
-------------------------
* Ubuntu 18.04/20.04 AMD64 or Ubuntu18.04/20.04 ARM 
* ROS [melodic](http://wiki.ros.org/melodic/Installation/Ubuntu) or ROS [noetic](http://wiki.ros.org/noetic/Installation)
* [libpanda](https://github.com/jmscslgroup/libpanda) (must be installed in /opt directory)

### Installation
---------------

1.Create a ROS Workspace
```
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
```
2.Clone the repo.

```
$ cd ~/catkin_ws/src/
$ git clone https://github.com/jmscslgroup/can_to_ros.git
```
Note: make sure to install libpanda before running ```catkin_make```.

3.Build the WS
```
$ cd ~/catkin_ws
$ catkin_make
```
4.Source your enviroment:
```
$source ~/catkin_ws/devel/setup.bash
```
## Tests:
-------
The following tests were performed on a Toyota RAV4

## 1-Sending Acceleration Commands to RAV4
We created a set of slides ([Step Input Commands Test](https://docs.google.com/presentation/d/1nBOYf58OQKMXlTMIj9IBpDr5be11WevXijcDsJr9ujI/edit#slide=id.p)) that have the steps needed to send acceleration commands to a Toyota RAV4.

## 2-Runnig Cruise Control Test
The goad of this test is to replicate the cruise control functionality with a PID controller. Steps to run the test are [here](https://docs.google.com/presentation/d/1rO9q0N9FP5X4kz3IavIRWEAJL3_q3MWPg--coTE5-Ps/edit#slide=id.p).
