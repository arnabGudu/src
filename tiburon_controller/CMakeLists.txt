cmake_minimum_required(VERSION 2.8.3)
project(tiburon_controller)

find_package(catkin REQUIRED COMPONENTS
  genmsg
  qt_build
  roscpp
  roslib	#required to use ros::package::getpath();
  #message_generation
)

add_message_files(
  FILES
  pid_tuning.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

## System dependencies are found with CMake's conventions
find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

################################################
## Find files 
################################################
file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/mainwindow.h)
SET(QT_SOURCES src/mainwindow.cpp src/array.cpp)

################################################
## Setup QT
################################################
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})


###################################
## catkin specific configuration ##
###################################
catkin_package(
  CATKIN_DEPENDS message_runtime
) 


###########
## Build ##
###########
include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${QT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(appLib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(appLib ${QT_LIBRARIES} ${catkin_LIBRARIES})
add_dependencies(appLib ${PROJECT_NAME}_generate_messages_cpp)

add_executable(app src/main.cpp)
target_link_libraries(app appLib)


