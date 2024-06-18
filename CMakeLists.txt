cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fortress_sim)

# Find the Gazebo packages
find_package(ignition-gazebo6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)

add_library(ecm_provider include/ECMProvider.h src/ECMProvider.cpp)
add_library(train_simulator include/Simulator.h src/Simulator.cpp)

target_link_libraries(train_simulator ecm_provider)

add_executable(launch_empty_world src/main.cpp)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
ignition-gazebo6::ignition-gazebo6
ignition-rendering6::ignition-rendering6
)