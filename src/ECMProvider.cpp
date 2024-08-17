#include "ECMProvider.h"

#include <iostream> // I use cout because I'm a peasant


/*
Constructor
*/
ECMProvider::ECMProvider(): gz::sim::System(){}

ECMProvider::~ECMProvider(){
    ecm_ptr_ = nullptr;
    evtmgr_ptr_ = nullptr;

}


/*
function that is run when the server does 
AddSystem() on this system
*/
void ECMProvider::Configure(const gz::sim::Entity& entity,
    const std::shared_ptr<const sdf::Element>&,
    gz::sim::EntityComponentManager& ecm,
    gz::sim::EventManager& eventMgr)
{

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
    this->evtmgr_ptr_ = &eventMgr; // point to the addy
    std::cout << "SUCCES: obtained EntityComponentManager & EvenManager from World: " << this->world_name_ << std::endl;
}

const gz::sim::EntityComponentManager* ECMProvider::getECM(){
    return this->ecm_ptr_;
}

const gz::sim::EventManager* ECMProvider::getEvtMgr(){
    return this->evtmgr_ptr_;
}