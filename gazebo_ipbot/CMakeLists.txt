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
  tuning.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(include)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_library(img_cap_lib include/img_cap.h src/img_cap.cpp)
add_executable( img_cap src/img_cap_main.cpp)
target_link_libraries(img_cap img_cap_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(img_cap img_cap_lib gazebo_ipbot_generate_messages_cpp)

add_library(cmd_vel_lib include/cmd_vel.h src/cmd_vel.cpp)
add_executable(cmd_vel src/cmd_vel_main.cpp)
target_link_libraries( cmd_vel cmd_vel_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(cmd_vel gazebo_ipbot_generate_messages_cpp)

add_library(img_cap_ard_lib include/img_cap.h src/img_cap_ard.cpp)
add_executable( img_cap_ard src/img_cap_main.cpp)
target_link_libraries(img_cap_ard img_cap_ard_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(img_cap_ard img_cap_ard_lib gazebo_ipbot_generate_messages_cpp)
