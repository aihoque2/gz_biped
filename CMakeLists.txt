cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fortress_sim)

# Find the Gazebo packages
find_package(ignition-gazebo6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)


add_executable(launch_empty_world src/main.cpp)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
ignition-gazebo6::ignition-gazebo6
ignition-rendering6::ignition-rendering6
)