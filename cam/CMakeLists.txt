cmake_minimum_required(VERSION 2.8.3)
project(cam)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  cv_bridge
  image_transport
)

find_package(OpenCV REQUIRED)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

include_directories(include)

add_library(cam_lib include/cam.h src/cam.cpp)
add_executable(cam src/main.cpp)
target_link_libraries(cam cam_lib ${OpenCV_LIBS} ${catkin_LIBRARIES})
