#include "StateUpdater.h"

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
    return;
}

void StateUpdater::PostUpdate()
{
    return;
}