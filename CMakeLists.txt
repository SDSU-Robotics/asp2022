# Project's name
PROJECT (asp2022)

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.1)

add_definitions(-std=c++11)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(catkin REQUIRED)
find_package (Threads REQUIRED)
find_package(catkin REQUIRED COMPONENTS
	roscpp
	rospy
	std_msgs
	sensor_msgs
	laser_geometry
	tf2
	tf2_ros
	geometry_msgs
	tf
	nav_msgs
)

catkin_package()

include_directories (src include ${catkin_INCLUDE_DIRS})

add_executable(motorController ${PROJECT_SOURCE_DIR}/src/motorController.cpp)
target_link_libraries(motorController ${catkin_LIBRARIES} Threads::Threads)

add_executable(pumpControl ${PROJECT_SOURCE_DIR}/src/pumpControl.cpp)
target_link_libraries(pumpControl ${catkin_LIBRARIES} Threads::Threads)

add_executable(ManualDriveBase ${PROJECT_SOURCE_DIR}/src/ManualDriveBase.cpp)
target_link_libraries(ManualDriveBase ${catkin_LIBRARIES} Threads::Threads)


target_link_libraries(pumpControl ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_Phoenix.a)
target_link_libraries(pumpControl ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixCCI.a)
target_link_libraries(pumpControl ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixPlatformLinuxSocketCan.a)
target_link_libraries(pumpControl ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixCanutils.a)

target_link_libraries(ManualDriveBase ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_Phoenix.a)
target_link_libraries(ManualDriveBase ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixCCI.a)
target_link_libraries(ManualDriveBase ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixPlatformLinuxSocketCan.a)
target_link_libraries(ManualDriveBase ${PROJECT_SOURCE_DIR}/lib/x86-64/libCTRE_PhoenixCanutils.a)


