#include "ContactSensor.h"

BipedalContact::BipedalContact(std::mutex& contactMutex, std::shared_ptr<bool[]> contacted){}

BipedalContact::~BipedalContact(){/* Documentation Inherited */}

BipedalContact::Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr){
    /*
    TODO

    your code here :)
    */

   // check to make sure the contact elements have been put on our desired links (idgaf about the other contacts)
    for (std::string link_name : LINK_NAMES){
        auto linkComp = ecm.EntityByComponents()
    }

}

BipedalContact::PreUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm){
    /*
    TODO

    your code here :)
    */
}