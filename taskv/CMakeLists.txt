cmake_minimum_required(VERSION 2.8.3)
project(taskv)

find_package(catkin REQUIRED COMPONENTS
  OpenCV
  roscpp
)

include_directories(

  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable( taskv src/taskv.cpp )
target_link_libraries( taskv ${OpenCV_LIBS} )

add_executable( img src/img.cpp )
target_link_libraries( img ${OpenCV_LIBS} )



