#include "EffortController.h"

namespace components = gz::sim::components;

EffortController::EffortController(std::mutex& axnMutex, std::shared_ptr<double[]> axn) 
: ignition::gazebo::System(), axn_mutex_(axnMutex), axn_(axn), forceCompCreation(0)
{
    for (auto name: JOINT_NAMES){
        std::cout << "here's name of a joint: " << name << '\n';
    }
}

EffortController::~EffortController()
{
    for (auto name : JOINT_NAMES){
        joint_map_[name] = nullptr;
    }
    std::cout << "num times ForceCompCreation was called: " << forceCompCreation << std::endl;
}

void EffortController::Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr)
{
    auto joints = ecm.EntitiesByComponents(gz::sim::components::Joint());
    // std::cout << "size of joints array: " << joints.size() << std::endl;

    // mapping with the hash
    for (auto joint : joints){
        if (ecm.EntityHasComponentType(joint, gz::sim::components::Name().typeId)){
            std::string name = ecm.Component<gz::sim::components::Name>(joint)->Data();
            std::cout << "Configure() name of joint: " << name << std::endl;

            // force command checking (Claim: avoid repeadetedely
            // doing this in PreUpdate(). Proof is by def of
            // PreUpdate() and Configure() systems). See cites.
            auto force = ecm.Component<gz::sim::components::JointForceCmd>(joint);

            if (force == nullptr){
                // create JointForceCmd for the joint
                ecm.CreateComponent(joint, gz::sim::components::JointForceCmd({0.0})); 
            }

            // insert to hashmap
            ignition::gazebo::Entity* entityPtr = &joint;
            joint_map_.insert(std::make_pair(name, entityPtr));
        }
    }

    std::cout << "joint_map_ creation completed. Here is the size: " << joint_map_.size() << std::endl << std::flush;
}

void EffortController::PreUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm)
{
    /*
    Torque msg updates
    */
    std::lock_guard<std::mutex> lock(axn_mutex_); // lock our axn array
    for (int i = 0; i < JOINT_NAMES.size(); i++){
        std::string jntNm = JOINT_NAMES[i];
        std::cout << "here's jntNm: " << jntNm << '\n';

        // null entity error check (prevent segfaults)
        if (joint_map_[jntNm] == nullptr){
            throw std::runtime_error("EffortController::PreUpdate(): entity: " + jntNm + " returned NULL. wtf");
            return;
        }

        gz::sim::Entity joint = *joint_map_[jntNm];
    
        // send force command
        // 07/25 refer to this on line 335: 
        // https://github.com/gazebosim/gz-sim/blob/gz-sim8/src/systems/joint_controller/JointController.cc
        
        auto force = ecm.Component<gz::sim::components::JointForceCmd>(joint);
        if (force == nullptr){
            std::cout << "EffortController::PreUpdate(): entity: " + jntNm + " has NULL force Component ptr. Adding force component..." << std::endl << std::flush;
            ecm.CreateComponent(joint, gz::sim::components::JointForceCmd({axn_[i]}));
            forceCompCreation++;
        }

        else{
            *force = gz::sim::components::JointForceCmd({axn_[i]});
        }
        axn_[i] = 0.0;
    }
}