#Author: Mattehew Nice
import json

#toyota_rav4 list of ROS pubs
toyota_rav4 = {37:{'steering_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         180:{'vel':[['std_msgs::Float64'],['SPEED']]},
         869:{'lead_dist': [['std_msgs::Float64'],['LEAD_DISTANCE']] ,'rel_vel':[['std_msgs::Float64'],['REL_SPEED']]},
         # 384:{'track_a0':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 385:{'track_a1':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 386:{'track_a2':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 387:{'track_a3':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 388:{'track_a4':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 389:{'track_a5':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 390:{'track_a6':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 391:{'track_a7':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 392:{'track_a8':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 393:{'track_a9':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 394:{'track_a10':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 395:{'track_a11':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 396:{'track_a12':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 397:{'track_a13':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 398:{'track_a14':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         # 399:{'track_a15':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
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
         ,'acc/set_distance':[['std_msgs::Int16'],['DISTANCE_LINES']]
         # 921:{'acc/distance_setting':[['std_msgs::Int16'],['DISTANCE_LINES']] # didn't write this - kj
         ,'acc/cruise_state':[['std_msgs::String'],['CRUISE_CONTROL_STATE']]
         ,'acc/cruise_state_int':[['std_msgs::Float64'],['CRUISE_CONTROL_STATE']]
         ,'acc/set_speed2':[['std_msgs::Float64'],['UI_SET_SPEED']]
         },
         552:{'accel':[['std_msgs::Float64'],['ACCEL_X']]}

}
#honda list
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
         303:{'vel':[['std_msgs::Float64'],['SPEED']]},
         308:{'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_STATE']]
         ,'acc/cruise_state':[['std_msgs::String'],['CRUISE_STATE']]
         ,'wheel_torque':[['std_msgs::Float64'],['WHEEL_TORQUE_CMD']]
         ,'brake_torque':[['std_msgs::Float64'],['BRAKE_TORQUE_CMD']]
         },
         1119:{'acc/acc_btns':[['std_msgs::String'],['ACC_BTNS']],
         'acc/acc_btns_int':[['std_msgs::Int16'],['ACC_BTNS']],
         #308:{'acc/cruise_state_int':[['std_msgs::Float64'],['CRUISE_STATE']]}, #didn't write -KJ
         #1119:{'acc_btns':[['std_msgs::Float64'],['ACC_BTNS']], # ddn't write - KJ
         #'unknown_btns':[['std_msgs::Float64'],['SIGNAL1']],
         'lkas_btn':[['std_msgs::Float64'],['LKAS_LDW_ON']]
         },
         1487:{'acc/set_speed':[['std_msgs::Int16'],['SET_SPEED']],
         #'lane_lines':[['std_msgs::Float64'],['LANE_LINES']],
         'acc/state_signal':[['std_msgs::Float64'],['STATE_SIGNAL']],
         'acc/set_distance':[['std_msgs::Int16'],['SET_DISTANCE']],
         'hud_states':[['std_msgs::Float64'],['HUD_STATES']],
         'acc/mini_car':[['std_msgs::Int16'],['MINICAR']],
         'system_on':[['std_msgs::Float64'],['SYSTEM_ON']]}
         # 'hud_state1':[['std_msgs::Float64'],['HUD_STATE1']],

         # 951: {"front_radar_track1": [["geometry_msgs::PointStamped"], ["SIG1_LEAD"]],
         # "front_radar_track2": [["geometry_msgs::PointStamped"], ["SIG2_VEL"]],
         # "front_radar_track3": [["geometry_msgs::PointStamped"], ["SIG3_ANGLEH"]],
         # "front_radar_track4": [["geometry_msgs::PointStamped"], ["SIG4_ANGLEV"]],
         # },
         # 954: {"front_radar_track1": [["geometry_msgs::PointStamped"], ["SIG1_LEAD"]],
         # "front_radar_track2": [["geometry_msgs::PointStamped"], ["SIG2_VEL"]],
         # "front_radar_track3": [["geometry_msgs::PointStamped"], ["SIG3_ANGLEH"]],
         # "front_radar_track4": [["geometry_msgs::PointStamped"], ["SIG4_ANGLEV"]],
         # },
         # 958: {"front_radar_track1": [["geometry_msgs::PointStamped"], ["SIG1_LEAD"]],
         # "front_radar_track2": [["geometry_msgs::PointStamped"], ["SIG2_VEL"]],
         # "front_radar_track3": [["geometry_msgs::PointStamped"], ["SIG3_ANGLEH"]],
         # "front_radar_track4": [["geometry_msgs::PointStamped"], ["SIG4_ANGLEV"]],
         # }
         } #others are 951,954,958
# nissan_rogue = {
# 	 427: {"bytes0": [["geometry_msgs::PointStamped"], ["BYTES0"]],
#         "bytes1": [["geometry_msgs::PointStamped"], ["BYTES4"]],
#         "bytes2": [["geometry_msgs::PointStamped"], ["BYTES8"]],
#         "bytes3": [["geometry_msgs::PointStamped"], ["BYTES12"]],
#         "bytes4": [["geometry_msgs::PointStamped"], ["BYTES16"]],
#         "bytes5": [["geometry_msgs::PointStamped"], ["BYTES20"]],
#         "bytes6": [["geometry_msgs::PointStamped"], ["BYTES24"]],
#         "bytes7": [["geometry_msgs::PointStamped"], ["BYTES28"]],
#         "bytes8": [["geometry_msgs::PointStamped"], ["BYTES32"]],
#         "bytes9": [["geometry_msgs::PointStamped"], ["BYTES36"]],
#         "bytes10": [["geometry_msgs::PointStamped"], ["BYTES40"]],
#         "bytes11": [["geometry_msgs::PointStamped"], ["BYTES44"]]
#          }
#     }

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
