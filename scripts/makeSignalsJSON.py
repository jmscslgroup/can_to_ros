import json

#toyota_rav4 list of ROS pubs
toyota_rav4 = {37:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         180:{'vel':[['std_msgs::Float64'],['SPEED']]},
         869:{'lead_dist': [['std_msgs::Float64'],['LEAD_DISTANCE']] ,'rel_vel':[['std_msgs::Float64'],['REL_SPEED']]},
         384:{'track_a0':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         385:{'track_a1':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         386:{'track_a2':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         387:{'track_a3':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         388:{'track_a4':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         389:{'track_a5':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         390:{'track_a6':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         391:{'track_a7':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         392:{'track_a8':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         393:{'track_a9':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         394:{'track_a10':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         395:{'track_a11':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         396:{'track_a12':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         397:{'track_a13':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         398:{'track_a14':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         399:{'track_a15':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         1570:{'highbeams':[['std_msgs::Float64'],['HIGH_BEAMS_ON']]},
         467:{'pcm_cruise_2':[['geometry_msgs::Point'],['MAIN_ON','SET_SPEED']]},
         921:{'cruise_state':[['geometry_msgs::Point'],['MAIN_ON','UI_SET_SPEED','CRUISE_CONTROL_STATE']]},
         552:{'accel':[['std_msgs::Float64'],['ACCEL_X']]}

}
#honda list
#nissan list
nissan_rogue = {139:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         303:{'vel':[['std_msgs::Float64'],['SPEED']]},
         381: {"radar_accels": [["geometry_msgs:PointStamped"], ["REL_ACCEL1","REL_ACCEL2","REL_ACCEL3"]],
         "radar_fourths": [["geometry_msgs:PointStamped"], ["FOURTH1","FOURTH2","FOURTH3"]],
         "radar_lead": [["geometry_msgs:PointStamped"], ["LEAD_DIST1","LEAD_DIST2","LEAD_DIST3"]],
         "radar_relvs": [["geometry_msgs:PointStamped"], ["REL_VEL1","REL_VEL2","REL_VEL3"]],
         "radar_states": [["geometry_msgs:PointStamped"], ["BUTTON_PRESS0","BUTTON_PRESS1","BUTTON_PRESS2"]]
         },
         771:{'sonar1A' : [['geometry_msgs::PointStamped'], ['SONAR1','SONAR2','SONAR3'] ],
         'sonar1B' : [['geometry_msgs::PointStamped'], ['SONAR4','SONAR5','SONAR6'] ],
         'sonar2A' : [['geometry_msgs::PointStamped'], ['SONAR7','SONAR8','SONAR9'] ],
         'sonar2B' : [['geometry_msgs::PointStamped'], ['SONAR10','SONAR11','SONAR12'] ],
         'sonar3A' : [['geometry_msgs::PointStamped'], ['SONAR13','SONAR14','SONAR15'] ],
         'sonar3B' : [['geometry_msgs::PointStamped'], ['SONAR16','SONAR17','SONAR18'] ],
         'sonar3C' : [['geometry_msgs::PointStamped'], ['SONAR19','SONAR20'] ]
            }
         }



pubslist = [toyota_rav4, nissan_rogue] #ordered list of dicts to become make/model JSONs

dest_files = ['./toyota_rav4.json','./nissan_rogue.json'] #make into ordered list for multiple vehicles

count = 0
for dest_file in dest_files:
    output_file = open(dest_file, 'w', encoding='utf-8')
    # for/ dic in pubslist[count]:
    json.dump(pubslist[count], output_file)
    output_file.write("\n")
    count +=1
    output_file.close()
