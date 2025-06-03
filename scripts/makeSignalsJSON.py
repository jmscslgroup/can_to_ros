#Author: Mattehew Nice
import json

#toyota_rav4 list of ROS pubs
toyota_rav4 = {
         36:{'car/state/imu_x_y_yaw':[['std_msgs::Float64MultiArray'],['ACCEL_X','ACCEL_Y','YAW_RATE']]},
         37:{'car/state/steering':[['std_msgs::Float64MultiArray'],['STEER_ANGLE','STEER_FRACTION','STEER_RATE']]},
         170:{'car/state/wheel_speeds':[['std_msgs::Float64MultiArray'],['WHEEL_SPEED_FR','WHEEL_SPEED_FL','WHEEL_SPEED_RR','WHEEL_SPEED_RL']]
         },
         180:{'/car/state/vel_x':[['std_msgs::Float64'],['SPEED']]},
         384:{'car/radar/track_a0':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         385:{'car/radar/track_a1':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         386:{'car/radar/track_a2':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         387:{'car/radar/track_a3':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         388:{'car/radar/track_a4':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         389:{'car/radar/track_a5':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         390:{'car/radar/track_a6':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         391:{'car/radar/track_a7':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         392:{'car/radar/track_a8':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         393:{'car/radar/track_a9':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         394:{'car/radar/track_a10':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         395:{'car/radar/track_a11':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         396:{'car/radar/track_a12':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         397:{'car/radar/track_a13':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         398:{'car/radar/track_a14':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         399:{'car/radar/track_a15':[['geometry_msgs::PointStamped'], ['LONG_DIST','LAT_DIST','REL_SPEED'] ]},
         467:{'pcm_cruise_2':[['geometry_msgs::Point'],['MAIN_ON','SET_SPEED']],
		'acc/set_speed' : [['std_msgs::Int16'], ['SET_SPEED']]
		},
        552:{'car/state/imu_x':[['std_msgs::Float64MultiArray'],['ACCEL_X']]
        },
        608:{'steer_torque_driver':[['std_msgs::Float64'],['STEER_TORQUE_DRIVER']],
        'steer_torque_eps':[['std_msgs::Float64'],['STEER_TORQUE_EPS']],
        'steer_override':[['std_msgs::Int16'],['STEER_OVERRIDE']]
        },
        740:{'steer_request':[['std_msgs::Int16'],['STEER_REQUEST']],
        'steer_torque_cmd':[['std_msgs::Float64'],['STEER_TORQUE_CMD']]
        },
         835:{'acc/accel_cmd':[['std_msgs::Float64'],['ACCEL_CMD']],
         'acc/acc_info':[['geometry_msgs::Point'],['MINI_CAR','CAR_AHEAD','CANCEL_REQ']],
         'acc/mini_car':[['std_msgs::Int16'],['MINI_CAR']],
         'acc/acc_cut_in':[['std_msgs::Int16'],['ACC_CUT_IN']],
         'acc/acc_malfunction':[['std_msgs::Int16'],['ACC_MALFUNCTION']]
         },
         869:{'lead_dist': [['std_msgs::Float64'],['LEAD_DISTANCE']] ,
         'rel_vel':[['std_msgs::Float64'],['REL_SPEED']],
         "acc/set_btn":[["std_msgs::Int16"],["SET_BTN"]],
         "acc/res_btn":[["std_msgs::Int16"],["RES_BTN"]],
         "acc/cancel_btn":[["std_msgs::Int16"],["CANCEL_BTN"]]
         },
         921:{'cruise_state':[['geometry_msgs::Point'],['MAIN_ON','UI_SET_SPEED','CRUISE_CONTROL_STATE']]
         ,'acc/set_distance':[['std_msgs::Int16'],['DISTANCE_LINES']]#this is the display, 0 is not showing
         ,'acc/cruise_state':[['std_msgs::String'],['CRUISE_CONTROL_STATE']]
         ,'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_CONTROL_STATE']]
         ,'acc/set_speed2':[['std_msgs::Float64'],['UI_SET_SPEED']]
         },
         956:{'car/state/sport_mode':[['std_msgs::Bool'],['SPORT_ON']]
         ,'car/state/eco_mode':[['std_msgs::Bool'],['ECON_ON']]
         },
         1570:{'highbeams':[['std_msgs::Float64'],['HIGH_BEAMS_ON']]}
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
         
         
         381: {'car/radar/track_L1_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L1_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L1_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L1_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L1_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L1_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         382: {'car/radar/track_R1_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R1_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R1_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R1_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R1_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R1_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         385: {'car/radar/track_L2_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L2_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L2_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L2_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L2_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L2_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         386: {'car/radar/track_R2_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R2_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R2_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R2_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R2_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R2_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         389: {'car/radar/track_L3_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L3_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L3_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L3_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L3_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L3_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         390: {'car/radar/track_R3_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R3_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R3_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R3_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R3_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R3_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         393: {'car/radar/track_L4_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L4_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L4_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L4_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L4_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L4_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         394: {'car/radar/track_R4_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R4_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R4_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R4_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R4_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R4_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         398: {'car/radar/track_L5_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L5_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L5_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L5_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L5_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L5_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         399: {'car/radar/track_R5_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R5_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R5_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R5_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R5_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R5_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         405: {'car/radar/track_L6_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L6_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L6_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L6_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L6_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L6_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         407: {'car/radar/track_R6_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R6_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R6_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R6_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R6_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R6_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         411: {'car/radar/track_L7_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L7_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L7_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L7_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L7_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L7_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         412: {'car/radar/track_R7_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R7_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R7_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R7_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R7_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R7_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         415: {'car/radar/track_L8_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L8_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L8_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L8_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L8_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L8_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         416: {'car/radar/track_R8_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R8_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R8_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R8_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R8_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R8_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         419: {'car/radar/track_L9_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L9_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L9_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L9_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L9_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L9_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         420: {'car/radar/track_R9_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R9_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R9_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R9_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R9_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R9_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         423: {'car/radar/track_L10_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_L10_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_L10_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_L10_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_L10_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_L10_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
               },
         
         425: {'car/radar/track_R10_1':[['geometry_msgs::PointStamped'], ['DISTANCE_B_1' , 'DISTANCE_A_1', 'VALID_A_1']],
               'car/radar/track_R10_2':[['geometry_msgs::PointStamped'], ['DISTANCE_B_2' , 'DISTANCE_A_2', 'VALID_A_2']],
               'car/radar/track_R10_3':[['geometry_msgs::PointStamped'], ['DISTANCE_B_3' , 'DISTANCE_A_3', 'VALID_A_3']],
               'car/radar/track_R10_4':[['geometry_msgs::PointStamped'], ['DISTANCE_B_4' , 'DISTANCE_A_4', 'VALID_A_4']],
               'car/radar/track_R10_5':[['geometry_msgs::PointStamped'], ['DISTANCE_B_5' , 'DISTANCE_A_5', 'VALID_A_5']],
               'car/radar/track_R10_6':[['geometry_msgs::PointStamped'], ['DISTANCE_B_6' , 'DISTANCE_A_6', 'VALID_A_6']],
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
         342:{'car/state/steering':[['std_msgs::Float64MultiArray'],['STEER_ANGLE','STEER_ANGLE_RATE']]},
#         342:{'steer_angle':[['std_msgs::Float64'],['STEER_ANGLE']]},
         344:{'car/state/vel_x':[['std_msgs::Float64'],['XMISSION_SPEED']]},
         506:{'acc/cruise_state_int':[['std_msgs::Int16'],['CRUISE_STATES']]},
         780:{'acc/set_speed':[['std_msgs::Int16'],['CRUISE_SPEED']]},
         419:{'gear':[['std_msgs::String'],['GEAR_SHIFTER']]},
#         422:{'acc/acc_btns':[['std_msgs::String'],['GEAR_SHIFTER']]},
         490:{'accel':[['std_msgs::Float64'],['LONG_ACCEL']]},
#         927:{'lead_dist':[['std_msgs::Float64'],['LEAD_DISTANCE']]}
         1040:{'car/radar/track_a0':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1041:{'car/radar/track_a1':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1042:{'car/radar/track_a2':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1043:{'car/radar/track_a3':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1044:{'car/radar/track_a4':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1045:{'car/radar/track_a5':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1046:{'car/radar/track_a6':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1047:{'car/radar/track_a7':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1090:{'car/radar/track_a8':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1091:{'car/radar/track_a9':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1092:{'car/radar/track_a10':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},
         1093:{'car/radar/track_a11':[['geometry_msgs::PointStamped'], ['LEAD_DISTANCE','LATERAL_LOCATION','REL_VEL'] ]},

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
