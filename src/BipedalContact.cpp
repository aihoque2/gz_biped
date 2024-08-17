#include "BipedalContact.h"

BipedalContact::BipedalContact(std::mutex& contactMutex, std::shared_ptr<bool[]> contacted){}

BipedalContact::~BipedalContact(){/* Documentation Inherited */}

BipedalContact::Configure(const gz::sim::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        gz::sim::EntityComponentManager& ecm,
                        gz::sim::EventManager& eventMgr){


   // check to make sure the contact elements have been put on our desired links (idgaf about the other contacts)
    for (std::string link_name : LINK_NAMES){
        auto linkEnt = ecm.EntityByComponents(gz::sim::components::Name(link_name), 
                                                gz::sim::components::Link());

        if (linkEnt== gz::sim::kNullEntity){
            throw std::runtime_error("BipedalContact::Configure() link component: " + link_name + "returned NULL");
        }

        // TODO: check the <link> tag for its <collision> tag and <contact/> tag

        if (!ecm.EntityHasComponentType(linkEnt, gz::sim::components::Collision())){
            throw std::runtime_error("Contact Sensor Link " + link_name + " has no <collision> component. give it a contact")            
        }
        
    }

}

BipedalContact::PreUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm){
    /*
    TODO

    your code here :)
    */
}