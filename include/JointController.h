
#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>
#include <vector>
#include <string>

/*
JointEffortController.h


joint effort controller for the bipedal. acts as plugin

but i'm not making a ignition transport node to publish the data.
*/

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


class JointController: public ignition::gazebo::System, 
                         public ignition::gazebo::ISystemConfigure,
                         public ignition::gazebo::ISystemPreUpdate 
{
    public:
        JointController(std::mutex &axnMutex);
        ~JointController();

        // inherited from ISystemConfigure ABC
        void Configure(const ignition::gazebo::Entity& entity,
                        const ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr);

        // inherited from ISystemPreUpdate ABC
        void PreUpdate(const gz::sim::UpdateInfo& info,
                        const gz::sim::EntityComponentManager& ecm);


    private:
        std::shared_ptr<double[]> action_;// commanded action vector for each joint, as sepcified in docs
        std::mutex axnMutex; // lock which function can modify the action ptr
};


