cmake_minimum_required(VERSION 2.8.3)
project(ip_room)


find_package(catkin REQUIRED COMPONENTS
  OpenCV
  roscpp
)

find_package(OpenCV REQUIRED)

catkin_package(

)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(ip_room src/ip_room.cpp)
target_link_libraries(ip_room ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(ip_room0 src/main.cpp)
target_link_libraries(ip_room0 ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(ip_room1 src/main1.cpp)
target_link_libraries(ip_room1 ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(ip_room2 src/main2.cpp)
target_link_libraries(ip_room2 ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(motion src/motion.cpp)
target_link_libraries(motion ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(single_obj_tracker src/single_obj_tracker.cpp)
target_link_libraries(single_obj_tracker ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(bgSub src/bgSub.cpp)
target_link_libraries(bgSub ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(basic_motion src/basic_motion.cpp)
target_link_libraries(basic_motion ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(concat src/concat.cpp)
target_link_libraries(concat ${OpenCV_LIBS} ${catkin_LIBRARIES} )

add_executable(concat_image src/concat_image.cpp)
target_link_libraries(concat_image ${OpenCV_LIBS} ${catkin_LIBRARIES} )
