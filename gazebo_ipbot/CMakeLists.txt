cmake_minimum_required(VERSION 2.8.3)
project(gazebo_ipbot)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  sensor_msgs
  cv_bridge
  image_transport
  message_generation
)

find_package(
   OpenCV REQUIRED
)

add_message_files(
  FILES
  pid.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable( img_cap src/img_cap.cpp)
target_link_libraries( img_cap ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(img_cap gazebo_ipbot_generate_messages_cpp)

add_executable( cmd_vel src/cmd_vel.cpp)
target_link_libraries( cmd_vel ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(cmd_vel gazebo_ipbot_generate_messages_cpp)

