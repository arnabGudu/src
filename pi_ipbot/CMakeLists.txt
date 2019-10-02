cmake_minimum_required(VERSION 2.8.3)
project(pi_ipbot)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  sensor_msgs
  cv_bridge
  image_transport
  genmsg
  qt_build
)

find_package(
   OpenCV REQUIRED
)

find_package(Qt4 REQUIRED COMPONENTS QtCore QtGui)
include(${QT_USE_FILE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/pi_controller/mainwindow.h)
SET(QT_SOURCES src/pi_controller/mainwindow.cpp)

QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

add_message_files(
  FILES
  pid_pi.msg
  error.msg
)
 
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${QT_INCLUDE_DIRS}
)

add_library(pi_control_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(pi_control_lib ${QT_LIBRARIES} ${catkin_LIBRARIES})

add_executable(app src/pi_controller/main.cpp)
target_link_libraries(app pi_control_lib)

add_library(pi_motor_lib include/pi_motor/pi_motor.h src/pi_motor/pi_motor.cpp)
add_executable(pi_motor src/pi_motor/main.cpp)
target_link_libraries(pi_motor pi_motor_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(pi_motor pi_motor_lib pi_ipbot_generate_messages_cpp)

add_library(pi_cam_lib include/pi_cam/pi_cam.h src/pi_cam/pi_cam.cpp)
add_executable(pi_cam src/pi_cam/main.cpp)
target_link_libraries(pi_cam pi_cam_lib ${OpenCV_LIBS} ${catkin_LIBRARIES} )
add_dependencies(pi_cam pi_cam_lib pi_ipbot_generate_messages_cpp)

