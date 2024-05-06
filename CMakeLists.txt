cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(harmonic_sim)

# Find the Gazebo packages
find_package(gz-sim8 REQUIRED)

add_executable(launch_empty_world src/main.cpp)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
  PRIVATE gz-sim8::gz-sim8)