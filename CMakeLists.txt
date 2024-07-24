cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fortress_sim)

# cmake_print_variables(PROJECT_SOURCE_DIR)


# we use std::optional here, we need 17
set(CMAKE_CXX_STANDARD 17)
#set(Boost_USE_MULTITHREADED ON)


# Find the Gazebo packages
find_package(ignition-gazebo6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)

# Find Boost for subprocess
find_package(Boost 1.74.0 REQUIRED)
if(Boost_FOUND)
  message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
else()
  message(FATAL_ERROR "Boost not found")
endif()

# EntityComponentManager Provider
add_library(ecm_provider include/ECMProvider.h src/ECMProvider.cpp)
target_include_directories(ecm_provider PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(ecm_provider ignition-gazebo6::ignition-gazebo6)

# Joint Controller 
add_library(joint_controller include/EffortController.h src/EffortController.cpp)
target_include_directories(joint_controller PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(joint_controller ignition-gazebo6::ignition-gazebo6)

# TrainSimulator
add_library(train_simulator include/Simulator.h src/Simulator.cpp include/ECMProvider.h include/EffortController.h)
#target_include_directories(train_simulator PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(train_simulator ecm_provider joint_controller ${Boost_LIBRARIES}) # link TrainSimulator with ECMProvider

# our executable (or binary if you're familiar with Bazel)
add_executable(launch_empty_world src/main.cpp)
target_link_libraries(launch_empty_world train_simulator)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
ignition-gazebo6::ignition-gazebo6
ignition-rendering6::ignition-rendering6
)