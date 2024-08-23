#include "BipedalContact.h"

BipedalContact::BipedalContact(std::mutex& contactMutex, std::shared_ptr<bool[]> contacted)
: gz::sim::System(), contact_mutex_(contactMutex), contacted_(contacted)
{

    for (auto name : LINK_NAMES){
        std::cout << "here's name of them links: " << name << '\n';
    }
}

BipedalContact::~BipedalContact(){/* Documentation Inherited */}

void BipedalContact::Configure(const gz::sim::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        gz::sim::EntityComponentManager& ecm,
                        gz::sim::EventManager& eventMgr){

    if (!ecm.HasComponentType(gz::sim::components::ContactSensor::typeId)){
        throw std::runtime_error("BipedalContact::Configure() no ContactSensor found...wtf");
        }
    else{
        std::cout << "BipedalContact::Configure() went through first if statement fine!" << std::endl;
    }


   // check to make sure the contact elements have been put on our desired links (idgaf about the other contacts)
    for (std::string link_name : LINK_NAMES){
        auto linkEnt = ecm.EntityByComponents(gz::sim::components::Name(link_name), 
                                                gz::sim::components::Link());

        if (linkEnt== gz::sim::kNullEntity){
            throw std::runtime_error("BipedalContact::Configure() link component: " + link_name + "returned NULL");
        }

        // Check the <link> tag for its <contact/> tag
        if (!ecm.EntityHasComponentType(linkEnt, gz::sim::components::ContactSensor::typeId)){
            throw std::runtime_error("Link: " + link_name + " on the blackbird robot is expected to have a Contact Sensor.");
        }

        // checking <link> tag for <collision/> tag
        if (!ecm.EntityHasComponentType(linkEnt, gz::sim::components::Collision::typeId)){
            throw std::runtime_error("Contact Sensor Link " + link_name + " has no <collision> component. give it a contact");         
        }

        std::vector<gz::sim::Entity> collisions = ecm.ChildrenByComponents(linkEnt, gz::sim::components::Collision());


        // create ContactSensorData components for the collision elements
        for (auto collision_ent: collisions){
            if (!ecm.EntityHasComponentType(collision_ent, gz::sim::components::ContactSensorData::typeId)){
                std::cout << "creating ContactSensorData for: " << link_name << std::endl;
                ecm.CreateComponent(collision_ent, gz::sim::components::ContactSensorData());
            }
        }
    }

}

void BipedalContact::PostUpdate(const gz::sim::UpdateInfo& info,
                        const gz::sim::EntityComponentManager& ecm){
    /*
    your code here :)

    check if each link has made a collision, and update the sensor data in contacted_
    */
    std::cout << "BipedalContact::PostUpdate() still needs to be implemented." << std::endl;

}