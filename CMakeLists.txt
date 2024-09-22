cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(fortress_sim)

# cmake_print_variables(PROJECT_SOURCE_DIR)


# we use std::optional here, we need 17
set(CMAKE_CXX_STANDARD 17)
#set(Boost_USE_MULTITHREADED ON)


# Find the Gazebo packages
find_package(gz-sim8 REQUIRED)
find_package(gz-math7 REQUIRED)
#find_package(gz-msgs10 REQUIRED)

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

target_link_libraries(ecm_provider gz-sim8::gz-sim8)

# Joint Controller 
add_library(joint_controller include/EffortController.h src/EffortController.cpp)
target_include_directories(joint_controller PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(joint_controller gz-sim8::gz-sim8)

# Contact Sensor
add_library(bipedal_contact include/BipedalContact.h src/BipedalContact.cpp)
target_include_directories(bipedal_contact PUBLIC
 $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

target_link_libraries(bipedal_contact gz-sim8::gz-sim8)

# TrainSimulator
add_library(train_simulator include/Simulator.h 
src/Simulator.cpp include/ECMProvider.h 
include/EffortController.h 
include/BipedalContact.h)
#target_include_directories(train_simulator PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(train_simulator 
ecm_provider 
joint_controller 
bipedal_contact 
${Boost_LIBRARIES}
gz-math7) # link TrainSimulator with ECMProvider

# our executable (or binary if you're familiar with Bazel)
add_executable(launch_empty_world src/main.cpp)
target_link_libraries(launch_empty_world train_simulator)

# Link against the Gazebo libraries
target_link_libraries(launch_empty_world
gz-sim8::gz-sim8
gz-sim8::gz-sim8
)