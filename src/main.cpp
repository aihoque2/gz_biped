#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <gz/rendering.hh>
#include <gz/gui.hh>
#include <iostream>
#include "Simulator.h"

#include <gz/sim/Entity.hh>
#include <gz/sim/components/Collision.hh>
#include <gz/sim/components/Link.hh>
#include <gz/sim/components/ContactSensor.hh>

// class ModelNameProvider: public {
    
// };

int main(int argc, char **argv)
{
    TrainSimulator my_sim(true);
    bool hasBlackbird = my_sim.server_->HasEntity("blackbird", 0);
    std::cout << "hasBlackbird: " << hasBlackbird << "\n";
    auto paused = my_sim.server_->Paused(); // need to auto bc it returns std::optional
    std::cout << "server paused: " << *paused << std::endl;

    // run the server
    std::vector<double> axn1 = {0.0, 0.0, 8.0, 0.0, 0.0, /* Left Js */   
                                        0.0, 0.0, 8.0, 0.0, 0.0 /* Right Js */}; 
    std::cout << "first run with forces....\n"; // debug
    
    /* Either or */
    // my_sim.server_->Run(true, 300, false);
    my_sim.StepFew(axn1, 30, 290);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "second run...." << std::endl; // debug
    my_sim.server_->Run(true, 300, false);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "third run...." << std::endl; // debug
    my_sim.server_->Run(true, 300, false);

    my_sim.Pause(); // quick test for me
    
    std::vector<std::string> LINK_NAMES = {"l_foot", "r_foot", "torso"};
    for (auto link_name : LINK_NAMES){
        auto link_ent = my_sim.ecm_->EntityByComponents(gz::sim::components::Name(link_name), 
                                                gz::sim::components::Link());

        if (link_ent == gz::sim::kNullEntity){
            throw std::runtime_error("main.cpp link component: " + link_name + "returned NULL");
        }

        std::vector<gz::sim::Entity> contact_sensors = my_sim.ecm_->EntitiesByComponents(gz::sim::components::ContactSensor());

        bool has_contact_sensor = contact_sensors.size() > 0;

        if (!has_contact_sensor){
            throw std::runtime_error("main.cpp: can't find ContactSensor()for link: " + link_name );
        }
        // gz::sim::Entity contact_ent = my_sim.ecm_->ChildrenByComponents(linkEnt, gz::sim::components::ContactSensor());

        // std::vector<gz::sim::Entity> collisions = my_sim.ecm_->ChildrenByComponents(link_ent, gz::sim::components::Collision());
        // std::cout << "Link name: " << link_name <<  ", length of collisions vector: " << collisions.size() << std::endl;
    
    }
    return 0;
}