#include "gz_bipedal/JointController.h"



std::vector<std::string> joint_names = {"l_hip_roll", 
                                        "l_hip_yaw", 
                                        "l_hip_pitch", 
                                        "l_knee", 
                                        "l_ankle",
                                        "r_hip_roll", 
                                        "r_hip_yaw", 
                                        "r_hip_pitch", 
                                        "r_knee", 
                                        "r_ankle"
                                        };


JointController::JointController(std::mutex& axnMutex, std::shared_ptr<double[]> axn) : ignition::gazebo::System()
{
    action_ = axn; // up the reference count
    axnMutex_ = axnMutex; // share the same mutex?
}

JointController::Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr){}