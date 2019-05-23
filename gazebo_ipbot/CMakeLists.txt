cmake_minimum_required(VERSION 2.8.3)
project(gazebo_ipbot)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  sensor_msgs
  cv_bridge
  image_transport
)

find_package(
   OpenCV REQUIRED
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable( img_cap src/img_cap.cpp)
target_link_libraries( img_cap ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable( cmd_vel src/cmd_vel.cpp)
target_link_libraries( cmd_vel ${OpenCV_LIBS} ${catkin_LIBRARIES} )


