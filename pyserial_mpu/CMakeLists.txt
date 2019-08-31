cmake_minimum_required(VERSION 2.8.3)
project(pyserial_mpu)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

catkin_package(

)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(pyserial_mpu src/pyserial_mpu.cpp)
target_link_libraries(pyserial_mpu ${OpenCV_LIBS} ${catkin_LIBRARIES} )
