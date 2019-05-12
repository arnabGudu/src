cmake_minimum_required(VERSION 2.8.3)
project(sample_qt_app)

find_package(catkin REQUIRED COMPONENTS
  qt_build
  roscpp
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
SET(QT_SOURCES src/mainwindow.cpp)

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
)

add_library(appLib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(appLib ${QT_LIBRARIES} ${catkin_LIBRARIES})

add_executable(app src/main.cpp)
target_link_libraries(app appLib)
catkin_install_python(PROGRAMS scripts/nat_pub.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
