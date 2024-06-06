#include "gz_bipedal/ECMProvider.h"
#include <iostream> // debugging reasons

#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>


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
    const ignition::gazebo::EntityComponentManager& ecm,
    const ignition::gazebo::EventManager& eventMgr)
{
    // TODO
    if (!ecm.EntityHasComponentType(entity, 
    ignition::gazebo::components::World::typeId)){
        std::cerr << "ECMProvider system was not inserted in a world element" << std::endl;
        return;
    }

    this->worldname = utils::getExistingComponentData<//
    ignition::gazebo::components::Name>(ecm, entity);

    this->ecm_ptr_ = &ecm; // point to the addy
    this->eventMgr_ptr_ = &eventMgr;
    std::cout << "SUCCES: obtained EntityComponentManager & EvenManager from World: " << this->worldname << << std::endl;
}