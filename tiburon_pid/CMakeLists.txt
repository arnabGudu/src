cmake_minimum_required(VERSION 2.8.3)
project(tiburon_pid)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  synchronizer
  std_msgs
  thruster_controller 
  tiburon_controller
  genmsg
)

add_message_files(
  FILES
  rqt.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime std_msgs)
 
include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_library(pid_lib include/tiburon_pid.h src/ros.cpp src/pid.cpp) 
add_executable( pid src/main.cpp)
target_link_libraries(pid pid_lib ${catkin_LIBRARIES} )
add_dependencies(pid_lib ${PROJECT_NAME}_generate_messages_cpp)

add_library(pid_test_lib include/tiburon_pid.h src/Tests/tiburon_pid_test.cpp)
add_executable( pid_test src/main.cpp)
target_link_libraries(pid_test pid_test_lib ${catkin_LIBRARIES} )

add_executable( pid_cmd src/Tests/tiburon_pid_cmd.cpp)
target_link_libraries(pid_cmd ${catkin_LIBRARIES} )

add_executable( pid_test1 src/Tests/tiburon_pid_test1.cpp)
target_link_libraries(pid_test1 ${catkin_LIBRARIES} )
