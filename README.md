# can_to_ros

can_to_ros is a ROS package that provide a real-time and offline decoding of CAN bus messages. It is also integrated with [libpanda](https://github.com/jmscslgroup/libpanda) to send control commands to Toyota RAV4.


### Software Requirements and Dependencies
-------------------------
* Ubuntu 18.04/20.04 AMD64 or Ubuntu18.04/20.04 ARM
* ROS [melodic](http://wiki.ros.org/melodic/Installation/Ubuntu) or ROS [noetic](http://wiki.ros.org/noetic/Installation)
* [libpanda](https://github.com/jmscslgroup/libpanda) (must be installed in /opt directory)

## Launch File Descriptions:
#### vehicle_control.launch
This is automatically installed in robot_upstart with libpanda's scripts/installCanToRos.sh script

Invokes the following:
1. vehicle_interface	- Main interface for libpanda's ToyotHandler for vehicle control
2. publishCommands	- Maintains a regular 100Hz publish rate from /cmd_accel_safe to /car/cruise/accel_input
3. node_status.py		- Regulary publishes a 0 to /cmd_accel to maintain libpanda's command heartbeat if other commands are not sent
4. launch/safety_checks.launch
 - cut_in_disengager	- Ensures that cars cutting in ahead do no coincide with large deceleration commands by disengaging the cruise controller.
 - cruise_start_safety_check - Ensures that acceleration commands start within safe bounds and that a lead vehicle is seen on engage requests (otherwise a disengage is invoked)

 5) topic_check.py		- Checks publish rate of /car/libpanda/controls_allowed to dispaly status on Blinkt kit

 Expected interface topics:

 /cmd_accel - std_msgs/Float64 - This is where control commands should be sent.  cut_in_disengager will ensure convert this to /cmd_accel_safe as needed, then publishCommands will convert /cmd_accel_safe to /car/cruise/accel_input
 /car/can/raw - std_msgs/String - Reports CAN data.  Use the node subs_fs.cpp to interpret these into car sensing data

#### piping.launch
This launch file allows you to pipe in replayed CAN and/or GPS data stored in CSV files, as if they are being read directly from the car in real time. Make sure that the generated subs.cpp node matches the vehicle type of the CSV file you are playing back data from. Caviat: the playback timing is not perfect in all cases but still very useful.

> Usage:
```
roslaunch can_to_ros piping.launch publish_csv_args:= "-c {/FILEPATH/DATE_VIN_CAN_Messages.csv} -g {/FILEPATH/DATE_VIN_GPS_Messages.csv}" bag_name:="my_bagfile"
```

Uses:
1. Creating new bagfiles for data. E.g. you've added new nodes and want to do software-in-the-loop testing.
2. Playback for [Ros2Ascii](https://github.com/jmscslgroup/ros2ascii) visualizations. Example snippet [here](https://www.youtube.com/watch?v=YoWxLxQu0Vs) for a Honda Pilot.

## Node Descriptions:

#### vehicle_interface.cpp
This ROS node interfaces libpanda's ToyotaHandler

Publishers:
1. /car/can/raw - std_msgs/String -  This publishes CAN data of interest where the can_to_ros node named subs_fs.cpp can interpret values
2. /car/panda/gas_interceptor_detected - std_msgs/Bool - Reported by the Panda is their gas interceptor hardware is detected
3. /car/panda/controls_allowed - std_msgs/Bool -  Reported by the comma.ai panda.  This is not event-based from the Panda, but is regularly checked at 2 Hz to reset the Panda's heartbeat
4.	/car/libpanda/controls_allowed - std_msgs/Bool -  Reported by logic within libpanda.  This is event based from libpanda using CAN messages.  When no events occur, this regularly published at 1 Hz which can be used to assess libpanda's control command health

Subscribers:
1. /car/cruise/accel_input - std_msgs/Float64 - This is for acceleration commands to be sent to the car's cruise controller (priorly known as /commands)
2. /car/hud/mini_car_enable - std_msgs/Bool - When true, this will display a mini-vehicle on the car's HUD which cruise control is on and engaged
3. /car/hud/cruise_cancel_request - std_msgs/Bool - When true  published, the cruise controller will disengage and notify the driver with an audible chime



### Installation
---------------

1. Create a ROS Workspace
```
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
```
2. Clone the repo.

```
$ cd ~/catkin_ws/src/
$ git clone https://github.com/jmscslgroup/can_to_ros.git
```
Note: make sure to install libpanda before running ```catkin_make```.

3. Build the WS
```
$ cd ~/catkin_ws
$ catkin_make
```
4. Source your enviroment:
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
