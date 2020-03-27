# Python and ROS for can_speed_decoder

~~~
$ mkdir catkin_ws # make a ros workspace
$ cd catkin_ws
$ mksir src
$ cd src
$ git clone https://github.com/SafwanElmadani/can_speed_decoder.git
$ cd ..
$ catkin_make
~~~

In one terminal window run the following:
~~~
$ cd catkin_ws
$ source devel/setup.bash
$ roslaunch can_speed_decoder publish_can_msg.launch
~~~

The above only listens to updated from a file in ~/can_data.csv

In another terminal window, run:
~~~
$ rosrun can_speed_decoder hello_speed.py 
~~~


