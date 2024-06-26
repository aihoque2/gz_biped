cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fortress_sim)

include(config.cmake)

# cmake_print_variables(PROJECT_SOURCE_DIR)


# we use std::optional here, we need 17
set(CMAKE_CXX_STANDARD 17)


# Find the Gazebo packages
find_package(ignition-gazebo6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)

# Find Boost for subprocess
find_package(Boost 1.74.0 REQUIRED)

add_library(ecm_provider include/ECMProvider.h src/ECMProvider.cpp)
target_include_directories(ecm_provider PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(ecm_provider ignition-gazebo6::ignition-gazebo6)


add_library(train_simulator include/Simulator.h src/Simulator.cpp)
# target_include_directories(train_simulator PUBLIC
#  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(train_simulator ecm_provider tiny-process-library) # link TrainSimulator with ECMProvider

add_executable(launch_empty_world src/main.cpp)

target_link_libraries(launch_empty_world train_simulator)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
ignition-gazebo6::ignition-gazebo6
ignition-rendering6::ignition-rendering6
)