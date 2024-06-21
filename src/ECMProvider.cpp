#include "ECMProvider.h"

#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>
#include <iostream> // I use cout because I'm a peasant


/*
Constructor
*/
ECMProvider::ECMProvider(): ignition::gazebo::System(){}


/*
function that is run when the server does 
AddSystem() on this system
*/
void ECMProvider::Configure(const gz::sim::Entity& entity,
    const std::shared_ptr<const sdf::Element>&,
    const ignition::gazebo::EntityComponentManager& ecm,
    ignition::gazebo::EventManager& eventMgr)
{
    // TODO
    if (!ecm.EntityHasComponentType(entity, 
    gz::sim::components::World::typeId)){
        std::cerr << "ECMProvider system was not inserted in a world entity" << std::endl;
        return;
    }

    // get our world name optional
    auto world_opt = ecm.ComponentData<gz::sim::components::Name>(entity);
    if (world_opt.has_value()){ // world_optional has string
        this->world_name_ = *std::move(world_opt);
    }
    else{
        std::cerr << "ECMProvider could not give the world name, even though we passed world entity" << std::endl;
        return;
    }
    

    this->ecm_ptr_ = &ecm; // point to the addy
    this->eventMgr_ptr_ = &eventMgr;
    std::cout << "SUCCES: obtained EntityComponentManager & EvenManager from World: " << this->world_name_ << std::endl;
}