cmake_minimum_required(VERSION 2.8.3)
project(qt_pi_line_follower)

set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  cv_bridge
  image_transport
  roscpp
  roslib
  sensor_msgs
  geometry_msgs
)

find_package(OpenCV REQUIRED)

include_directories(

  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(camera src/camera.cpp)
target_link_libraries(camera ${OpenCV_LIBS} ${catkin_LIBRARIES} )
