#include "gz_bipedal/ECMProvider.h"
#include <iostream> // debugging reasons


/*
Constructor
*/
ECMProvider::ECMProvider(): ignition::gazebo::System(){}


/*
function that is run when the server does 
AddSystem() on this system
*/
void ECMProvider::Configure(const ignition::gazebo::Entity& entity,
    const std::shared_ptr<const sdf::Element>&,
    ignition::gazebo::EntityComponentManager& ecm,
    ignition::gazebo::EventManager& eventMgr)
{
    // TODO
    this->ecm_ptr_ = &ecm; // point to the addy
    this->eventMgr_ptr_ = &eventMgr;
}