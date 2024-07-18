#include "JointController.h"

typedef gz::sim::components components;

JointController::JointController(std::mutex& axnMutex, std::shared_ptr<double[]> axn) : ignition::gazebo::System()
{
    action_ = axn; // up the reference count
    axn_mutex_ = axnMutex; // share the same mutex?

    for (auto name: JOINT_NAMES){
        std::cout << "here's name of a joint: " << name << std::endl;
    }

}

JointController::~JointController(std::mutex& axnMutex, std::shared_ptr<double[]> axn) : ignition::gazebo::System()
{
    for (auto name : JOINT_NAMES){
        joint_map_[name] = nullptr;
    }

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
                        gz::sim::EntityComponentManager& ecm)
{
    /*
    Torque msg updates
    */
    std::lock_guard<std::mutex> lock(axnMutex_); // lock our axn array
    for (int i = 0; i < JOINT_NAMES.size(); i++){
        std::string jntNm = JOINT_NAMES[i];
        std::cout << "here's jntNm: " << jntNm << std::endl;

        // null entity error check (prevent segfaults)
        if (joint_map_[jntNm] == nullptr){
            throw std::runtime_error("JointController::PreUpdate(): entity: " + jntNm + " returned NULL. wtf");
        }

        gz::sim::Entity joint = *joint_map_[jntNm];
        

        // send force command       
        auto force = ecm_.Component<components::JointForceCmd>(joint);
        force->Data()[0] += axn_[i];
        axn_[i] = 0.0;
    }
}