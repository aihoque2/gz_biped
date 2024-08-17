
#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <gz/rendering.hh>
#include <gz/sim/components/Joint.hh>
#include <gz/sim/components/JointForceCmd.hh>
#include <gz/sim/components/Name.hh>
#include <memory>
#include <vector>
#include <string>

/*
EffortController.h


joint effort controller for the bipedal. acts as plugin
but i'm not making a ignition transport node to publish the data.

cites:
like gz-sim::systems::ApplyJointForce
*/


#ifndef EFFORTCONTROLLER_HPP
#define EFFORTCONTROLLER_HPP


static const std::vector<std::string> JOINT_NAMES = 
{"l_hip_roll", "l_hip_yaw", "l_hip_pitch", "l_knee", "l_ankle", 
"r_hip_roll", "r_hip_yaw", "r_hip_pitch", "r_knee", "r_ankle"};

class EffortController: public gz::sim::System, 
                         public gz::sim::ISystemPreUpdate,
                         public gz::sim::ISystemConfigure
{
    public:
        EffortController(std::mutex& axnMutex, std::shared_ptr<double[]> axn);
        ~EffortController();

        // inherited from ISystemConfigure ABC
        void Configure(const gz::sim::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        gz::sim::EntityComponentManager& ecm,
                        gz::sim::EventManager& eventMgr);

        // inherited from ISystemPreUpdate ABC
        void PreUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm);



    private:
        std::shared_ptr<double[]> axn_; // commanded action vector for each joint, as sepcified in docs
        std::mutex& axn_mutex_; // lock which function can modify the action ptr
        int forceCompCreation;

};

#endif