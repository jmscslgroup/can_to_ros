cmake_minimum_required(VERSION 2.8.3)
project(can_to_ros)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  sensor_msgs
  message_generation
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
   FILES
   can_msgs.msg
   headlights.msg
#   Message2.msg
 )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
 )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
   INCLUDE_DIRS include  #uncomment
#  LIBRARIES can_speed_decoder
#  CATKIN_DEPENDS roscpp rospy std_msgs
CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/can_speed_decoder.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/can_speed_decoder_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_can_speed_decoder.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)

if(EXISTS /usr/local/include/panda.h)
	message(" - libpanda found!")
	set(LIBPANDA_FOUND YES)
else()
	message(" - libpanda NOT found")
endif()

if(DEFINED LIBPANDA_FOUND)
	INCLUDE_DIRECTORIES(
		/opt/libpanda/include
		/opt/libpanda/include/panda
		/opt/libpanda/include/mogi
		/opt/libpanda/NMEAParser/Software/NMEAParserLib/include)

	LINK_DIRECTORIES(
		/opt/libpanda/build
		/opt/libpanda/build/utilities )
endif()

add_executable(gpsfix src/gps_node.cpp)
target_link_libraries(gpsfix ${catkin_LIBRARIES})


add_executable(publish_csv src/publish_csv.cpp)
target_link_libraries(publish_csv ${catkin_LIBRARIES})

add_executable(subs src/subs.cpp)
target_link_libraries(subs ${catkin_LIBRARIES})

add_executable(subs_fs src/subs_fs.cpp)
target_link_libraries(subs_fs ${catkin_LIBRARIES})

add_executable(subs_fs_test src/subs_fs_test.cpp)
target_link_libraries(subs_fs_test ${catkin_LIBRARIES})

add_executable(subs_nissan_side_radar src/subs_nissan_side_radar.cpp)
target_link_libraries(subs_nissan_side_radar ${catkin_LIBRARIES})

add_executable(lead_info src/lead_info.cpp)
target_link_libraries(lead_info ${catkin_LIBRARIES})

if(DEFINED LIBPANDA_FOUND)
	add_executable(publish_libpanda src/publish_libpanda.cpp)
	target_link_libraries(publish_libpanda ${catkin_LIBRARIES} libpandac.so) #libpandac.so
endif()

add_executable(realtime_publishing src/realtime_publishing.cpp)
target_link_libraries(realtime_publishing ${catkin_LIBRARIES})

#add_executable(sendCommands src/sendCommands.cpp)
#target_link_libraries(sendCommands ${catkin_LIBRARIES} libpandac.so)


add_executable(commandDuration src/commandDuration.cpp)
target_link_libraries(commandDuration ${catkin_LIBRARIES})


add_executable(publishCommands src/publishCommands.cpp)
target_link_libraries(publishCommands ${catkin_LIBRARIES})


add_executable(publishCommandsFS src/publishCommandsFS.cpp)
target_link_libraries(publishCommandsFS ${catkin_LIBRARIES})

add_executable(cmd_accel_decel src/cmd_accel_decel.cpp)
target_link_libraries(cmd_accel_decel ${catkin_LIBRARIES})

if(DEFINED LIBPANDA_FOUND)
	add_executable(vehicle_interface src/vehicle_interface.cpp)
	target_link_libraries(vehicle_interface ${catkin_LIBRARIES} libpandac.so)

	add_executable(vanilla_vehicle_interface src/vanilla_vehicle_interface.cpp)
	target_link_libraries(vanilla_vehicle_interface ${catkin_LIBRARIES} libpandac.so)
endif()


add_executable(nissan_target_speed_to_acc_buttons src/nissan_target_speed_to_acc_buttons.cpp)
target_link_libraries(nissan_target_speed_to_acc_buttons ${catkin_LIBRARIES} )

add_executable(simple_mini_car_from_lead_distance src/simple_mini_car_from_lead_distance.cpp)
target_link_libraries(simple_mini_car_from_lead_distance ${catkin_LIBRARIES})

add_executable(cruise_start_safety_check src/cruise_start_safety_check.cpp)
target_link_libraries(cruise_start_safety_check ${catkin_LIBRARIES})

if(DEFINED LIBPANDA_FOUND)
	add_executable(cut_in_disengager src/cut_in_disengager.cpp)
	target_link_libraries(cut_in_disengager ${catkin_LIBRARIES} libpandac.so)

	add_executable(ramp src/ramp.cpp)
	target_link_libraries(ramp ${catkin_LIBRARIES} libpandac.so)
endif()

add_executable(cbf_smoother src/cbf_smoother.cpp)
target_link_libraries(cbf_smoother ${catkin_LIBRARIES})

# add_executable(joyNode src/joyNode.cpp)
# target_link_libraries(joyNode ${catkin_LIBRARIES})

catkin_install_python(PROGRAMS scripts/listener.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
catkin_install_python(PROGRAMS scripts/can_coach.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/can_coach_subs.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/selector.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/node_status.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/topic_check.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)




# message( " @@@@@@@@@@@@@@@@@@@@@@@ " ${PROJECT_SOURCE_DIR})
