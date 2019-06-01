cmake_minimum_required(VERSION 2.8.3)
project(gazebo_ipbot_qt)

find_package(catkin REQUIRED COMPONENTS
  qt_build
  roscpp
  std_msgs
  geometry_msgs
  sensor_msgs
  cv_bridge
  image_transport
  message_generation
)

find_package(
   OpenCV REQUIRED
)

add_message_files(
  FILES
  pid.msg
  tuning.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/mainwindow.h)
SET(QT_SOURCES src/mainwindow.cpp)

QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_library(gazebo_ipbot_qt_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(gazebo_ipbot_qt_lib ${QT_LIBRARIES} ${OpenCV_LIBS} ${catkin_LIBRARIES})

add_executable(gazebo_ipbot_qt src/main.cpp)
target_link_libraries(gazebo_ipbot_qt gazebo_ipbot_qt_lib)
add_dependencies(gazebo_ipbot_qt gazebo_ipbot_qt_lib gazebo_ipbot_qt_generate_messages_cpp)
