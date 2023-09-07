#Author: Mattehew Nice
import json

#toyota_rav4 list of ROS pubs
toyota_rav4 = {37:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         180:{'vel':[['std_msgs::Float64'],['SPEED']]},
         869:{'lead_dist': [['std_msgs::Float64'],['LEAD_DISTANCE']] ,
         'rel_vel':[['std_msgs::Float64'],['REL_SPEED']],
         "acc/set_btn":[["std_msgs::Int16"],["SET_BTN"]],
         "acc/res_btn":[["std_msgs::Int16"],["RES_BTN"]],
         "acc/cancel_btn":[["std_msgs::Int16"],["CANCEL_BTN"]]
         },
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
         467:{'pcm_cruise_2':[['geometry_msgs::Point'],['MAIN_ON','SET_SPEED']]
		,'acc/set_speed' : [['std_msgs::Int16'], ['SET_SPEED']]
		},
         835:{'acc/accel_cmd':[['std_msgs::Float64'],['ACCEL_CMD']],
         'acc/acc_info':[['geometry_msgs::Point'],['MINI_CAR','CAR_AHEAD','CANCEL_REQ']],
         'acc/mini_car':[['std_msgs::Int16'],['MINI_CAR']],
         'acc/acc_cut_in':[['std_msgs::Int16'],['ACC_CUT_IN']],
         'acc/acc_malfunction':[['std_msgs::Int16'],['ACC_MALFUNCTION']]
         },
         921:{'cruise_state':[['geometry_msgs::Point'],['MAIN_ON','UI_SET_SPEED','CRUISE_CONTROL_STATE']]
         ,'acc/set_distance':[['std_msgs::Int16'],['DISTANCE_LINES']]#this is the display, 0 is not showing
         ,'acc/cruise_state':[['std_msgs::String'],['CRUISE_CONTROL_STATE']]
         ,'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_CONTROL_STATE']]
         ,'acc/set_speed2':[['std_msgs::Float64'],['UI_SET_SPEED']]
         },
         552:{'accel':[['std_msgs::Float64'],['ACCEL_X']]
         },
         740:{'steer_request':[['std_msgs::Int16'],['STEER_REQUEST']],
         'steer_torque_cmd':[['std_msgs::Float64'],['STEER_TORQUE_CMD']]
         },
         608:{'steer_torque_driver':[['std_msgs::Float64'],['STEER_TORQUE_DRIVER']],
         'steer_torque_eps':[['std_msgs::Float64'],['STEER_TORQUE_EPS']],
         'steer_override':[['std_msgs::Int16'],['STEER_OVERRIDE']]
         }

}

#nissan list
# nissan_rogue = {139:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
#          303:{'vel':[['std_msgs::Float64'],['SPEED']]},
#          423: {"side_radar_track1": [["geometry_msgs::PointStamped"], ["LEAD_DIST1","BUTTON_PRESS0","BUTTON_PRESS1"]],
#          "side_radar_track2": [["geometry_msgs::PointStamped"], ["LEAD_DIST2"]],
#          "side_radar_track3": [["geometry_msgs::PointStamped"], ["LEAD_DIST3"]],
#          "side_radar_track4": [["geometry_msgs::PointStamped"], ["LEAD_DIST4"]],
#          "side_radar_track5": [["geometry_msgs::PointStamped"], ["LEAD_DIST5"]],
#          "side_radar_track6": [["geometry_msgs::PointStamped"], ["LEAD_DIST6"]],
#          },
# 	 425: {"side_radar_track8": [["geometry_msgs::PointStamped"], ["LEAD_DIST1","BUTTON_PRESS0","BUTTON_PRESS1"]],
#          "side_radar_track9": [["geometry_msgs::PointStamped"], ["LEAD_DIST2"]],
#          "side_radar_track10": [["geometry_msgs::PointStamped"], ["LEAD_DIST3"]],
#          "side_radar_track11": [["geometry_msgs::PointStamped"], ["LEAD_DIST4"]],
#          "side_radar_track12": [["geometry_msgs::PointStamped"], ["LEAD_DIST5"]],
#          "side_radar_track13": [["geometry_msgs::PointStamped"], ["LEAD_DIST6"]],
#          },
#          771:{'sonar1A' : [['geometry_msgs::PointStamped'], ['SONAR1','SONAR2','SONAR3'] ],
#          'sonar1B' : [['geometry_msgs::PointStamped'], ['SONAR4','SONAR5','SONAR6'] ],
#          'sonar2A' : [['geometry_msgs::PointStamped'], ['SONAR7','SONAR8','SONAR9'] ],
#          'sonar2B' : [['geometry_msgs::PointStamped'], ['SONAR10','SONAR11','SONAR12'] ],
#          'sonar3A' : [['geometry_msgs::PointStamped'], ['SONAR13','SONAR14','SONAR15'] ],
#          'sonar3B' : [['geometry_msgs::PointStamped'], ['SONAR16','SONAR17','SONAR18'] ],
#          'sonar3C' : [['geometry_msgs::PointStamped'], ['SONAR19','SONAR20'] ]
#             }
#          }
nissan_rogue = {139:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         # 116:{'accel_pedal_driver':[['std_msgs::Float64'],['ACCEL_PEDAL_DRIVER']]},
         140:{'accel_pedal_driver':[['std_msgs::Float64'],['ACCEL_PEDAL_POSITION']]},
         303:{'vel':[['std_msgs::Float64'],['SPEED']]},
         308:{'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_STATE']]
         ,'acc/cruise_state':[['std_msgs::String'],['CRUISE_STATE']]
         ,'wheel_torque':[['std_msgs::Float64'],['WHEEL_TORQUE_CMD']]
         ,'brake_torque':[['std_msgs::Float64'],['BRAKE_TORQUE_CMD']]
         },
         # 548:{'brake_pedal':[['std_msgs::Float64'],['BRAKE_PEDAL']]},
         1119:{'acc/acc_btns':[['std_msgs::String'],['ACC_BTNS']],
         'acc/acc_btns_int':[['std_msgs::Int16'],['ACC_BTNS']],
         #'unknown_btns':[['std_msgs::Float64'],['SIGNAL1']],
         # 'lkas_btn':[['std_msgs::Float64'],['LKAS_LDW_ON']]
         },
         1487:{'acc/set_speed':[['std_msgs::Int16'],['SET_SPEED']],
         #'lane_lines':[['std_msgs::Float64'],['LANE_LINES']],
         'acc/state_signal':[['std_msgs::Float64'],['STATE_SIGNAL']],
         'acc/set_distance':[['std_msgs::Int16'],['SET_DISTANCE']],
         'hud_states':[['std_msgs::Float64'],['HUD_STATES']],
         'acc/mini_car':[['std_msgs::Int16'],['MINICAR']]}#,,


         } #others are 951,954,958

#honda list
honda_pilot={
         342:{'steer_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         344:{'vel':[['std_msgs::Float64'],['XMISSION_SPEED']]},
         506:{'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_STATES']]},
         780:{'acc/set_speed':[['std_msgs::Int16'],['CRUISE_SPEED']]},
         419:{'gear':[['std_msgs::String'],['GEAR_SHIFTER']]},
#         422:{'acc/acc_btns':[['std_msgs::String'],['GEAR_SHIFTER']]},
         490:{'accel':[['std_msgs::Float64'],['LONG_ACCEL']]},
         927:{'lead_dist':[['std_msgs::Float64'],['LEAD_DISTANCE']]}

         }

pubslist = [toyota_rav4, nissan_rogue, honda_pilot] #ordered list of dicts to become make/model JSONs

dest_files = ['./toyota_rav4.json','./nissan_rogue.json', './honda_pilot.json'] #make into ordered list for multiple vehicles

count = 0
for dest_file in dest_files:
    output_file = open(dest_file, 'w', encoding='utf-8')
    # for/ dic in pubslist[count]:
    json.dump(pubslist[count], output_file)
    output_file.write("\n")
    count +=1
    output_file.close()
