# Python and ROS for can_speed_decoder

~~~
$ mkdir catkin_ws # make a ros workspace
$ cd catkin_ws
$ mksir src
$ cd src
$ git clone https://github.com/SafwanElmadani/can_speed_decoder.git
$ git checkout ROS_libpanda
$ cd ..
$ catkin_make
~~~

# Runnin hello_speed.py

Now run the roslaunch file for can_speed_decoder:
~~~
$ cd catkin_ws	# only if you are not already in catkin_ws
$ source devel/setup.bash
$ roslaunch can_speed_decoder publish_can_msg.launch
~~~

The above only listens to updated from a file in ~/can_data.csv.  To populate, run pandacord in a new terminal:

~~~
$ sudo pandacord -c ~/can_data.csv
~~~

Finally, in yet another terminal windows (3rd one so far) run:
~~~
$ cd catkin_ws
$ source devel/setup.bash
$ rosrun can_speed_decoder hello_speed.py 
~~~


# speed_talker.py

If you do not have a vehicle and Grey Panda handy, or want to test the above without needing hardware, do the following to see code in action:

~~~
$ roscore # roslaunch invokes this automatically, but we don't use roslaunch in this example
~~~

In a 2nd terminal, run speed_talker.py:
~~~
$ cd catkin_ws
$ source devel/setup.bash
$ rosrun can_speed_decoder speed_talker.py 
~~~


Finally in a 3rd terminal, run hello_speed.py:
~~~
$ rosrun can_speed_decoder hello_speed.py
~~~
