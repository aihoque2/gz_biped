#include "StateUpdater.h"
#include "EffortController.h" // for the JOINT_NAMES list
/*
implement that header stuff here
*/

StateUpdater::StateUpdater(std::shared_ptr<double[]> states)
:ignition::gazebo::System()
{
    state_ = states;
}

void StateUpdater::Configure()
{   
    // TODO: read the github code and see what's
    // needed to be done here.
    return;
}

void StateUpdater::PostUpdate(const UpdateInfo &_info,
                                const EntityComponentManager &ecm)
{
    gz::sim::Entity blackbird_ent = ecm.EntityByComponents(gz::sim::components::Name("blackbird"));
    
    // first 6 dimensions of state_
    auto pose_comp = ecm.Component<gz::sim::components::Pose>(blackbird_ent)
    // 3 states
    auto lin_vel_comp = ecm.Component<gz::sim::components::LinearVelocity>(blackbird_ent)
    // 3 states
    auto ang_vel_comp = ecm.Component<gz::sim::components::AngularVelocity>(blackbird_ent)
    // TODO: Joint states

    return;
}