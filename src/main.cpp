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
    
    std::vector<double> axn2 = {0.0, 0.0, 0.0, 8.0, 0.0, /* Left Js */   
                                        0.0, 0.0, 0.0, 8.0, 0.0 /* Right Js */};

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
    my_sim.ResetSim();

    std::cout << "fourth run...." << std::endl; // debug
    my_sim.server_->Run(true, 100, false);
    my_sim.StepFew(axn2, 300, 500); 
    //my_sim.server_->Run(true, 700, false);

    //std::this_thread::sleep_for(std::chrono::seconds(3));

    my_sim.ResetSim();

    std::cout << "fifth run...." << std::endl;
    my_sim.server_->Run(true, 500, false);
    // my_sim.Pause(); // quick test for me

    return 0;
}