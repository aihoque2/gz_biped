#include "StateUpdater.h"
#include "EffortController.h" // for the JOINT_NAMES list
/*
implement that header stuff here
*/

StateUpdater::StateUpdater(std::shared_ptr<double[]> states)
:gz::sim::System(), gz::sim::ISystemConfigure, gz::sim::ISystemPostUpdate, gz::sim::IsystemReset
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
    int i = 0;

    // first 6 dimensions of state_
    auto pose_comp = ecm.Component<gz::sim::components::Pose>(blackbird_ent)
    auto pose = pose_comp->Data()
    state_[0] = pose.Pos().X();
    state_[1] = pose.Pos().Y();
    state_[2] = pose.Pos().Z();

    // angular coords
    state_[3] = pose.Rot().Roll();
    state_[4] = pose.Rot().Pitch();
    state_[5] = pose.Rot().Yaw();
    i = 6;    

    auto lin_vel_comp = ecm.Component<gz::sim::components::LinearVelocity>(blackbird_ent)
    // 3 states
    lin_vel = lin_vel_comp->Data();
    for (; i < 9; i++){
        state_[i] = lin_vel[i-6]; // at i=11, we got ang_vel[2]
    }
    auto ang_vel_comp = ecm.Component<gz::sim::components::AngularVelocity>(blackbird_ent);
    for (; i < 12; i++){
        state_[i] = ang_vel[i-9]; // at i=11, we got ang_vel[2]
    }
    // TODO: Joint position states
    i = 12;
    for (auto joint_name: JOINT_NAMES){
        auto joint_ent = ecm.EntityByComponents(gz::sim::components::Joint(), gz::sim::components::Name(joint_name));
        auto j_pos = ecm.Component<gz::sim::components::JointPosition>(joint_ent);
        auto j_vel = ecm.Component<gz::sim::components::JointVelocity>(joint_ent);

        // set joint position
        state_[i] = j_pos->Data()[0];

        // set joint velocity
        state_[i+10] = j_vel->Data()[0];
        i++;
    }
    
    std::cout << "here's i: " << i << std::endl;
    return;
}