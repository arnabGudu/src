cmake_minimum_required(VERSION 2.8.3)
project(imgtransport)

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  cv_bridge
  image_transport
  roscpp
  roslib
  sensor_msgs
)

#catkin_package(CATKIN_DEPENDS std_msgs)

include_directories(

  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


add_executable( itnode1 src/itnode1.cpp )
target_link_libraries( itnode1 ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable( itnode2 src/itnode2.cpp )
target_link_libraries( itnode2 ${OpenCV_LIBS} ${catkin_LIBRARIES})
