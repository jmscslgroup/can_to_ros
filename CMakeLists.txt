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
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

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
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

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


# add_executable(speed_node src/speed_node.cpp)
# target_link_libraries(speed_node ${catkin_LIBRARIES})

# add_executable(strangle_node src/strangle_node.cpp)
# target_link_libraries(strangle_node ${catkin_LIBRARIES})

# add_executable(leaddist_node src/leaddist_node.cpp)
# target_link_libraries(leaddist_node ${catkin_LIBRARIES})

# add_executable(accel_node src/accel_node.cpp)
# target_link_libraries(accel_node ${catkin_LIBRARIES})

add_executable(gpsfix src/gps_node.cpp)
target_link_libraries(gpsfix ${catkin_LIBRARIES})

# add_executable(track_a0 src/track_a0.cpp)
# target_link_libraries(track_a0 ${catkin_LIBRARIES})

# add_executable(track_a1 src/track_a1.cpp)
# target_link_libraries(track_a1 ${catkin_LIBRARIES})

# add_executable(track_a2 src/track_a2.cpp)
# target_link_libraries(track_a2 ${catkin_LIBRARIES})

# add_executable(track_a3 src/track_a3.cpp)
# target_link_libraries(track_a3 ${catkin_LIBRARIES})

# add_executable(track_a4 src/track_a4.cpp)
# target_link_libraries(track_a4 ${catkin_LIBRARIES})

# add_executable(track_a5 src/track_a5.cpp)
# target_link_libraries(track_a5 ${catkin_LIBRARIES})

# add_executable(track_a6 src/track_a6.cpp)
# target_link_libraries(track_a6 ${catkin_LIBRARIES})

# add_executable(track_a7 src/track_a7.cpp)
# target_link_libraries(track_a7 ${catkin_LIBRARIES})

# add_executable(track_a8 src/track_a8.cpp)
# target_link_libraries(track_a8 ${catkin_LIBRARIES})

# add_executable(track_a9 src/track_a9.cpp)
# target_link_libraries(track_a9 ${catkin_LIBRARIES})

# add_executable(track_a10 src/track_a10.cpp)
# target_link_libraries(track_a10 ${catkin_LIBRARIES})

# add_executable(track_a11 src/track_a11.cpp)
# target_link_libraries(track_a11 ${catkin_LIBRARIES})

# add_executable(track_a12 src/track_a12.cpp)
# target_link_libraries(track_a12 ${catkin_LIBRARIES})

# add_executable(track_a13 src/track_a13.cpp)
# target_link_libraries(track_a13 ${catkin_LIBRARIES})

# add_executable(track_a14 src/track_a14.cpp)
# target_link_libraries(track_a14 ${catkin_LIBRARIES})

# add_executable(track_a15 src/track_a15.cpp)
# target_link_libraries(track_a15 ${catkin_LIBRARIES})

# add_executable(track_b0 src/track_b0.cpp)
# target_link_libraries(track_b0 ${catkin_LIBRARIES})

# add_executable(track_b1 src/track_b1.cpp)
# target_link_libraries(track_b1 ${catkin_LIBRARIES})

# add_executable(track_b2 src/track_b2.cpp)
# target_link_libraries(track_b2 ${catkin_LIBRARIES})

# add_executable(track_b3 src/track_b3.cpp)
# target_link_libraries(track_b3 ${catkin_LIBRARIES})

# add_executable(track_b4 src/track_b4.cpp)
# target_link_libraries(track_b4 ${catkin_LIBRARIES})

# add_executable(track_b5 src/track_b5.cpp)
# target_link_libraries(track_b5 ${catkin_LIBRARIES})

# add_executable(track_b6 src/track_b6.cpp)
# target_link_libraries(track_b6 ${catkin_LIBRARIES})

# add_executable(track_b7 src/track_b7.cpp)
# target_link_libraries(track_b7 ${catkin_LIBRARIES})

# add_executable(track_b8 src/track_b8.cpp)
# target_link_libraries(track_b8 ${catkin_LIBRARIES})

# add_executable(track_b9 src/track_b9.cpp)
# target_link_libraries(track_b9 ${catkin_LIBRARIES})

# add_executable(track_b10 src/track_b10.cpp)
# target_link_libraries(track_b10 ${catkin_LIBRARIES})

# add_executable(track_b11 src/track_b11.cpp)
# target_link_libraries(track_b11 ${catkin_LIBRARIES})

# add_executable(track_b12 src/track_b12.cpp)
# target_link_libraries(track_b12 ${catkin_LIBRARIES})

# add_executable(track_b13 src/track_b13.cpp)
# target_link_libraries(track_b13 ${catkin_LIBRARIES})

# add_executable(track_b14 src/track_b14.cpp)
# target_link_libraries(track_b14 ${catkin_LIBRARIES})

# add_executable(track_b15 src/track_b15.cpp)
# target_link_libraries(track_b15 ${catkin_LIBRARIES})

add_executable(publish_csv src/publish_csv.cpp)
target_link_libraries(publish_csv ${catkin_LIBRARIES})

add_executable(subs src/subs.cpp)
target_link_libraries(subs ${catkin_LIBRARIES})

catkin_install_python(PROGRAMS scripts/listener.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)














