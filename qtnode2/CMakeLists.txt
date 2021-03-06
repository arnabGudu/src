cmake_minimum_required(VERSION 2.8.3)
project(qtnode2)

find_package(catkin REQUIRED COMPONENTS
  qt_build
  roscpp
  cv_bridge
  image_transport
  roslib
  sensor_msgs
)

## System dependencies are found with CMake's conventions
find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

################################################
## Find files 
################################################
file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/qtnode2.h)
SET(QT_SOURCES src/qtnode2.cpp)

################################################
## Setup QT
################################################
QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})


###################################
## catkin specific configuration ##
###################################
catkin_package()

###########
## Build ##
###########
include_directories(include)
include_directories(
	${catkin_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS}
  	${OpenCV_INCLUDE_DIRS}
)

add_library(appLib2 ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(appLib2 ${QT_LIBRARIES} ${catkin_LIBRARIES})

add_executable(app2 src/main2.cpp)
target_link_libraries(app2 appLib2)
