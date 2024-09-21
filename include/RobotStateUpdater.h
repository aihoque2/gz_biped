#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <memory>
#include <vector>
#include <string>

/*
StateUpdater.h

I'm copying part of this:
https://github.com/gazebosim/gz-sim/blob/ign-gazebo6/src/systems/joint_state_publisher/JointStatePublisher.hh

but i'm not making a ignition transport node to publish the data.
*/




class StateUpdater: public ignition::gazebo::System, 
                         public ignition::gazebo::ISystemConfigure,
                         public ignition::gazebo::ISystemPostUpdate 
{
    public:
        StateUpdater();
        ~StateUpdater();

        // inherited from ISystemConfigure ABC
        void Configure(const ignition::gazebo::Entity& entity,
                        const ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr);

        // inherited from ISystemPostUpdate ABC
        void PostUpdate(const ignition::gazebo::UpdateInfo& info,
                        const ignition::gazebo::EntityComponentManager& ecm);


    private:
        std::shared_ptr<double[]> state_ // joint states vector, as sepcified in docs

};


