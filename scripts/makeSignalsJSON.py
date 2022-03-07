import json

# #toyota_hybrid list of signals
# toDecode = {180:['SPEED'], 384:['LONG_DIST','LAT_DIST','REL_SPEED'], 552:['ACCEL_X']}
#
# diclist = [toDecode]
#
# dest_file = './toyota_hybrid_header.json' #make into ordered list for multiple vehicles
#
# output_file = open(dest_file, 'w', encoding='utf-8')
# for dic in diclist:
#     json.dump(dic, output_file)
#     output_file.write("\n")


#toyota_hybrid list of ROS pubs
toROS = {180:{'vel':[['std_msgs::Float64'],['SPEED']]},
         869:{'lead_dist': [['std_msgs::Float64'],['LEAD_DISTANCE']] ,'rel_vel':[['std_msgs::Float64'],['REL_SPEED']]},
         384:{'track_a0':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         552:{'accel':[['std_msgs::Float64'],['ACCEL_X']]}    
}

pubslist = [toROS]

dest_file = './toyota_hybrid.json' #make into ordered list for multiple vehicles

output_file = open(dest_file, 'w', encoding='utf-8')
for dic in pubslist:
    json.dump(dic, output_file)
    output_file.write("\n")

# {'accel_cmd':[['PointStamped'],{384:['LONG_DIST','LAT_DIST','REL_SPEED'],400:['REL_ACCEL']}]}

# accel_pub = n_.advertise<std_msgs::Float64>("accel", 1000);
# lead_dist_pub = n_.advertise<std_msgs::Float64>("lead_dist_869", 1000);
# relative_vel_pub = n_.advertise<std_msgs::Float64>("rel_vel_869", 1000);
# str_angle_pub = n_.advertise<std_msgs::Float64>("steering_angle", 1000);
# speed_pub = n_.advertise<geometry_msgs::Twist>("vel", 1000);
# pub_921 = n_.advertise<geometry_msgs::Point>("msg_921", 1000);
# pub_467 = n_.advertise<geometry_msgs::Point>("msg_467", 1000);
# tracka0_pub = n_.advertise<geometry_msgs::PointStamped>("track_a0", 1000);
# tracka1_pub = n_.advertise<geometry_msgs::PointStamped>("track_a1", 1000);
# tracka2_pub = n_.advertise<geometry_msgs::PointStamped>("track_a2", 1000);
# tracka3_pub = n_.advertise<geometry_msgs::PointStamped>("track_a3", 1000);
# tracka4_pub = n_.advertise<geometry_msgs::PointStamped>("track_a4", 1000);
# tracka5_pub = n_.advertise<geometry_msgs::PointStamped>("track_a5", 1000);
# tracka6_pub = n_.advertise<geometry_msgs::PointStamped>("track_a6", 1000);
# tracka7_pub = n_.advertise<geometry_msgs::PointStamped>("track_a7", 1000);
# tracka8_pub = n_.advertise<geometry_msgs::PointStamped>("track_a8", 1000);
# tracka9_pub = n_.advertise<geometry_msgs::PointStamped>("track_a9", 1000);
# tracka10_pub = n_.advertise<geometry_msgs::PointStamped>("track_a10", 1000);
# tracka11_pub = n_.advertise<geometry_msgs::PointStamped>("track_a11", 1000);
# tracka12_pub = n_.advertise<geometry_msgs::PointStamped>("track_a12", 1000);
# tracka13_pub = n_.advertise<geometry_msgs::PointStamped>("track_a13", 1000);
# tracka14_pub = n_.advertise<geometry_msgs::PointStamped>("track_a14", 1000);
# tracka15_pub = n_.advertise<geometry_msgs::PointStamped>("track_a15", 1000);
