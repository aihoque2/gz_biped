#include "gz_bipedal/JointController.h"



// order our joint commands will be recieved in
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

void JointController::Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr)
{
    auto joints = ecm_->EntitiesByComponents(gz::sim::components::Joint());
    // std::cout << "size of joints array: " << joints.size() << std::endl;

    for (auto joint : joints){
        if (ecm_->EntityHasComponentType(joint, gz::sim::components::Name().typeId)){
            std::string name = ecm_->Component<gz::sim::components::Name>(joint)->Data();
            std::cout << "name of joint: " << name << std::endl;

            ignition::gazebo::Entity* entityPtr = &joint;

            jointMap.insert(std::make_pair(name, entityPtr));
        }
    }

    std::cout << "jointMap_ creation completed. Here is the size: " << jointMap_.size() << std::endl;
}

void JointController::PreUpdate(const gz::sim::UpdateInfo& info,
                        const gz::sim::EntityComponentManager& ecm)
{
    /*
    Torque msg updates
    */
    std::lock_guard<std::mutex> lock(axnMutex_); // lock our axn array
    for (int i =0; i < joint_names.size(); i++){
        
    }

}