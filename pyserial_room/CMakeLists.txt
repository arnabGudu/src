cmake_minimum_required(VERSION 2.8.3)
project(pyserial_room)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(pyroom src/main.cpp)
target_link_libraries(pyroom ${catkin_LIBRARIES} )
