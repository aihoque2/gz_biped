#include "gz_bipedal/JointStateUpdater.h"

/*
implement that header stuff here
*/

JointStateUpdater::JointStateUpdater(std::shared_ptr<double[]> states):ignition::gazebo::System()
{
    state_ = states;
}

void JointStateUpdater::Configure()
{   
    return;
}

void JointStateUpdater::PostUpdate()
{
    return;
}