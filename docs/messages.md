# can_to_ros Messages

This document lists the ROS topics and corresponding nodes, as well as if controller designers can have access to various elements.  

 - Private: Read-only
 - Public: Intended for controller designers to use


/accel
 - std_msgs/Float64          

## /car
 - These messages are represente din an ontology of a single vehicle with abstracted values from lower-level protocols

#### /car/cruise
 - These messages correspond to states and interfaces of the vehicle's cruise controller ifrastructure

/car/cruise/accel_input - Values publised here will be sent to the cruise controller for the desired acceleration
 - std_msgs/Float64    
 - Access: private
 - Subscribers: vehicle_interface, sendCommands
 - Publishers: publishComands, publishCommandsFS

#### /car/hud
 - These messages correspond to interactable HUD elements.  Some also affect the state of /car/cruise

/car/hud/cruise_cancel_request - Sending a "1" will invoke a cruise controller cancellation, informing the driver and disabling the cruise controller
 - std_msgs/Bool
 - Access: Public
 - Subscribers:  vehicle_interface
 - Publishers: cruise_start_safety_check, cut_in_disengager

/car/hud/mini_car_enable - This will display the "Mini Car" on the HUD, the HUD element that indicates a leader vehicle is recognized
 - std_msgs/Bool         
 - Access: Private (due to conflicting node simple_mini_car_from_lead_distance, can be changed if needed)
 - Subscribers: cruise_start_safety_check, vehicle_interface
 - Publishers: simple_mini_car_from_lead_distance

#### /car/libpanda
 - This set of messages are produced from states within libpanda

/car/libpanda/controls_allowed - Indicates when control values from /car/cruise/accel_input are passed to the panda, and thus the vehicle's cruise controller.  This publishes at 1Hz when libpanda is healthy, but will also publish changes in value immediately.
 - std_msgs/Bool             
 - Access: Private
 - Subscribers: cruise_start_safety_check
 - Publishers: vehicle_interface

#### /car/panda
 - The panda USB device reports information.  These values are directyl from the panda firmware

/car/panda/controls_allowed - This is a report if the control commands are allowed to be sent.  This should not be used, see /car/libpanda/controls_allowed instead.  This should publish at an approximate 2Hz on correctly connected hardware, so this can be used to check for potential hardware issues.
 - std_msgs/Bool             
 - Access: Private
 - Subscribers: None
 - Pubslishers: vehicle_interface

/car/panda/gas_interceptor_detected - This is a report if special comma.ai hardware is connected.  This is not a reflection of vehicle state
 - std_msgs/Bool             
 - Access: Private
 - Subscribers: None
 - Publishers: vehicle_interface

/car/panda/gps_active (this should be moved to somewhere else in /car/) Indicates when GPS is active and system time has been synchronized.  Logic is handled by vehicle_interface
 - std_msgs/Bool             
 - Access: Private
 - Subscribers: rosbag_record.sh
 - Publishers: vehicle_interface

## Other
 - These topis will eventually be integrated into an ontology under /car

/realtime_raw_data - This is a report of specific CAN messages of interest for the vehicle sensor and control state.  See vehicle_interface for specific messages of interest.  The node subs_fs is responsible for parsing this data into separate message topics in more usable types
 - std_msgs/String 
 - Access: Private
 - Subscribers: subs_fs
 - Publishers: vehicle_interface


/cmd_accel - This is the requested desired acceleration.  The node cut_in_disengager reads this and passes through the values to /cmd_accel_safe assuming cut-in safety checks pass.  node_status.py will regularly publish acceleration of 0 to this topic if expected nodes are not running (this will be deprecated).
 - std_msgs/Float64       
 - Access: Public
 - Subscribers: cruise_start_safety_check, cut_in_disengager, publishCommands
 - Publishers: node_status.py  


/cmd_accel_safe - This is the "safe" requested desired acceleration as necessariy adjusted by the cut_in_disengager.  The node publishCommads reads this value and publishes the most recently-read value at regular intervals into /car/cruise/accel_input
 - std_msgs/Float64          
 - Access: Private
 - Subscribers: publishCommands
 - Publishers: cut_in_disengager


/cmd_vel - The commanded velocty, as an input to the FollowerStopper
 - geometry_msgs/Twist       
 - Access: Public
 - Subscribers:
 - Publishers:


/highbeams - Corresponds to the state of highbeams.  Can be used as a driver interface device.
 - std_msgs/Float64        
 - Access: Private
 - Subscribers: None
 - Publishers: subs_fs



/lead_dist - this was replaced by /lead_dist_869 (see below)
 - std_msgs/Float64     


/lead_dist_869 - This reports the processed lead distance message from the CAN bus, a parsed value from message 869 based on Toyota vehicles.  When this value == 252, no lead vehicle is in view.
 - std_msgs/Float64        
 - Access: Private
 - Subscribers: cut_in_disengager, lead_info, simple_mini_car_from_lead_distance, lead_info
 - Publishers: subs_fs


/msg_467 - Raw message data
 - geometry_msgs/Point         
 - Access: Private
 - Subscribers: None
 - Publishers: lead_info   

/msg_921 - Raw message Data
 - geometry_msgs/Point         
 - Access: Private
 - Subscribers: None
 - Publishers: lead_info   

/region - Not in can_to_ros
 - std_msgs/UInt8          
 - Access:
 - Subscribers: 
 - Publishers:     

/rel_vel - deprecated, see /rel_vel_869 below
 - geometry_msgs/Twist       
 - Access: Private
 - Subscribers: 
 - Publishers:     

/rel_vel_869 - The relative velocity of the leader vehicle, as produced by Toyota's ACC algorithm.  This exists in CAN bus message 869
 - std_msgs/Float64          
 - Access: Private
 - Subscribers: lead_info
 - Publishers: subs_fs       

/rel_vel_old
 - geometry_msgs/Twist       
 - Access: Private
 - Subscribers: None
 - Publishers: lead_info      
      
/steering_angle - The current steering angle of rthe vehicle
 - std_msgs/Float64   
 - Access: Private
 - Subscribers: None
 - Publishers: subs_fs       


/timheadway1 - Not in can_to_ros
 - std_msgs/Float64    


/track_a0 ... /track_a15 - Corresponds to raw data from the radar, unprocessed by Toyota's ACC
 - geometry_msgs/PointStamped
 - Access: Private
 - Subscribers: None
 - Publishers: subs_fs


/v_ref - Not in can_to_ros
 - geometry_msgs/Twist       


/vel - The speed reported by the vehicle
 - geometry_msgs/Twist 
 - Access: Private
 - Subscribers: None
 - Publishers: subs_fs
