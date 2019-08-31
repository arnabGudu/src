cmake_minimum_required(VERSION 2.8.3)
project(face_recog)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

find_package(
   OpenCV REQUIRED
)

include_directories(include)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_library(face_recog_lib include/face_recog.h src/face_recog.cpp)
add_executable( face_recog src/face_recog_main.cpp)
target_link_libraries( face_recog face_recog_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
