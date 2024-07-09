#include "gz_bipedal/JointController.h"

typedef gz::sim::components components;

// order our joint commands will be recieved in
std::vector<std::string> JOINT_NAMES = {"l_hip_roll", 
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
    axn_mutex_ = axnMutex; // share the same mutex?

}

void JointController::Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr)
{
    auto joints = ecm_.EntitiesByComponents(components::Joint());
    // std::cout << "size of joints array: " << joints.size() << std::endl;

    // mapping with the hash
    for (auto joint : joints){
        if (ecm_->EntityHasComponentType(joint, components::Name().typeId)){
            std::string name = ecm_->Component<components::Name>(joint)->Data();
            std::cout << "name of joint: " << name << std::endl;

            // force command checking (Claim: avoid repeadetedely
            // doing this in PreUpdate(). Proof is by def of
            // PreUpdate() and Configure() systems). See cites.
            auto force = ecm_.Component<components::JointForceCmd>(joint);

            if (force == nullptr){
                // create JointForceCmd for the joint
                ecm_.CreateComponent(joint, components::JointForceCmd({0.0})); 
            }

            // insert to hashmap
            ignition::gazebo::Entity* entityPtr = &joint;
            joint_map_.insert(std::make_pair(name, entityPtr));
        }
    }

    std::cout << "joint_map_ creation completed. Here is the size: " << joint_map_.size() << std::endl;
}

void JointController::PreUpdate(const gz::sim::UpdateInfo& info,
                        const gz::sim::EntityComponentManager& ecm)
{
    /*
    Torque msg updates
    */
    std::lock_guard<std::mutex> lock(axnMutex_); // lock our axn array
    for (int i = 0; i < JOINT_NAMES.size(); i++){
        std::string jntNm = JOINT_NAMES[i];
        gz::sim::Entity entityPtr = joint_map_[jntNm];
        
        // null entity error
        if (entityPtr == nullptr){
            throw std::runtime_error("JointController::PreUpdate(): entity: " + jntNm + " returned NULL");
        }


    }
}