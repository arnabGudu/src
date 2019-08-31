cmake_minimum_required(VERSION 2.8.3)
project(ip_robosub)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

find_package(OpenCV REQUIRED)

catkin_package(

)

include_directories(

  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(ip_robosub src/ip_robosub.cpp)
target_link_libraries(ip_robosub ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(test src/test.cpp)
target_link_libraries(test ${OpenCV_LIBS} ${catkin_LIBRARIES} )
