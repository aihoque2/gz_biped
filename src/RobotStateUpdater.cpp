#include "RobotStateUpdater.h"
#include "EffortController.h" // for the JOINT_NAMES list
/*
implement that header stuff here
*/

StateUpdater::StateUpdater(std::mutex& stateMutex, std::shared_ptr<double[]> state)
:gz::sim::System(), state_mutex_(stateMutex), state_(state) 
{
}

StateUpdater::~StateUpdater(){

}

void StateUpdater::Configure(const gz::sim::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&,
                        gz::sim::EntityComponentManager& ecm,
                        gz::sim::EventManager& eventMgr)
{   
    // read the github code and see what's
    // needed to be done here.

    gz::sim::Entity blackbird_ent = ecm.EntityByComponents(gz::sim::components::Name("blackbird"));
    auto* pose_comp = ecm.Component<gz::sim::components::Pose>(blackbird_ent);
    if (pose_comp == nullptr){
        throw std::runtime_error("StateUpdater::Configure() Pose component returned nullptr");
    }

    gz::math::Vector3d zero_vel(0, 0, 0);

    // linear velocity check
    if (!ecm.EntityHasComponentType(blackbird_ent, gz::sim::components::LinearVelocity().TypeId())){
        ecm.CreateComponent(blackbird_ent, gz::sim::components::LinearVelocity(zero_vel));
    }

    // angular velocity check
    if (!ecm.EntityHasComponentType(blackbird_ent, gz::sim::components::AngularVelocity().TypeId())){
        ecm.CreateComponent(blackbird_ent, gz::sim::components::AngularVelocity(zero_vel));
    }

    for (auto joint_name: JOINT_NAMES){
        gz::sim::Entity joint_ent = ecm.EntityByComponents(gz::sim::components::Joint(), gz::sim::components::Name(joint_name));
        auto* j_pos = ecm.Component<gz::sim::components::JointPosition>(joint_ent);
        auto* j_vel = ecm.Component<gz::sim::components::JointVelocity>(joint_ent);

        if (j_pos == nullptr){
            ecm.CreateComponent(joint_ent, gz::sim::components::JointPosition({0.0}));
        }
        if (j_vel == nullptr){
            ecm.CreateComponent(joint_ent, gz::sim::components::JointVelocity({0.0}));
        }
    }

}

void StateUpdater::PostUpdate(const gz::sim::UpdateInfo &_info,
                                const gz::sim::EntityComponentManager &ecm)
{
    gz::sim::Entity blackbird_ent = ecm.EntityByComponents(gz::sim::components::Name("blackbird"));
    int i = 0;

    // first 6 dimensions of state_
    auto* pose_comp = ecm.Component<gz::sim::components::Pose>(blackbird_ent);
    auto pose = pose_comp->Data();
    state_[0] = pose.Pos().X();
    state_[1] = pose.Pos().Y();
    state_[2] = pose.Pos().Z();

    // angular coords
    state_[3] = pose.Rot().Roll();
    state_[4] = pose.Rot().Pitch();
    state_[5] = pose.Rot().Yaw();
    i = 6;    

    auto* lin_vel_comp = ecm.Component<gz::sim::components::LinearVelocity>(blackbird_ent);
    // 3 states
    if (lin_vel_comp == nullptr){
        throw std::runtime_error("StateUpdater::PostUpdate() entity has no component LinearVelocity");
    }
    auto lin_vel = lin_vel_comp->Data();
    for (; i < 9; i++){
        std::cout << "in lin vel update here's i: " << i << std::endl;
        state_[i] = lin_vel[i-6]; // at i=11, we got ang_vel[2]
    }
    auto ang_vel_comp = ecm.Component<gz::sim::components::AngularVelocity>(blackbird_ent);
    if (ang_vel_comp == nullptr){
        throw std::runtime_error("StateUpdater::PostUpdate() entity has no component AngularVelocity");
    }
    
    auto ang_vel = ang_vel_comp->Data();
    for (; i < 12; i++){
        state_[i] = ang_vel[i-9]; // at i=11, we got ang_vel[2]
    }
    // Joint position states and vels
    i = 12;
    for (auto joint_name: JOINT_NAMES){
        gz::sim::Entity joint_ent = ecm.EntityByComponents(gz::sim::components::Joint(), gz::sim::components::Name(joint_name));
        auto* j_pos = ecm.Component<gz::sim::components::JointPosition>(joint_ent);
        if (j_pos == nullptr){
            throw std::runtime_error("Joint " + joint_name + " has no component JointPosition");
        }
        auto* j_vel = ecm.Component<gz::sim::components::JointVelocity>(joint_ent);
        if (j_vel == nullptr){
            throw std::runtime_error("Joint " + joint_name + " has no component JointVelocity");
        }

        // set joint position
        state_[i] = j_pos->Data()[0];

        // set joint velocity
        state_[i+10] = j_vel->Data()[0];
        i++; // i == 22 at the end of this loop, so we never access beyond idx 31
    }
    
    std::cout << "StateUpdater::PostUpdate() here's some state info: " << state_[25] << std::endl;
}

void StateUpdater::Reset(const gz::sim::UpdateInfo &info,
                 gz::sim::EntityComponentManager &ecm)
{
    gz::sim::Entity blackbird_ent = ecm.EntityByComponents(gz::sim::components::Name("blackbird"));
    auto* pose_comp = ecm.Component<gz::sim::components::Pose>(blackbird_ent);
    if (pose_comp == nullptr){
        throw std::runtime_error("StateUpdater::Reset() Pose component returned nullptr");
    }

    gz::math::Vector3d zero_vel(0, 0, 0);

    // linear velocity check
    if (!ecm.EntityHasComponentType(blackbird_ent, gz::sim::components::LinearVelocity().TypeId())){
        ecm.CreateComponent(blackbird_ent, gz::sim::components::LinearVelocity(zero_vel));
    }

    // angular velocity check
    if (!ecm.EntityHasComponentType(blackbird_ent, gz::sim::components::AngularVelocity().TypeId())){
        ecm.CreateComponent(blackbird_ent, gz::sim::components::AngularVelocity(zero_vel));
    }

    for (auto joint_name: JOINT_NAMES){
        gz::sim::Entity joint_ent = ecm.EntityByComponents(gz::sim::components::Joint(), gz::sim::components::Name(joint_name));
        auto* j_pos = ecm.Component<gz::sim::components::JointPosition>(joint_ent);
        auto* j_vel = ecm.Component<gz::sim::components::JointVelocity>(joint_ent);

        if (j_pos == nullptr){
            ecm.CreateComponent(joint_ent, gz::sim::components::JointPosition({0.0}));
        }
        if (j_vel == nullptr){
            ecm.CreateComponent(joint_ent, gz::sim::components::JointVelocity({0.0}));
        }
    }
}