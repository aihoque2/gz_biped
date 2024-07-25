#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <iostream>
#include "Simulator.h"


// class ModelNameProvider: public {
    
// };

int main(int argc, char **argv)
{
    TrainSimulator my_sim(true);
    bool hasBlackbird = my_sim.server_->HasEntity("blackbird", 0);
    std::cout << "hasBlackbird: " << hasBlackbird << std::endl;
    auto paused = my_sim.server_->Paused(); // need to auto bc it returns std::optional
    std::cout << "server paused: " << *paused << std::endl;

    // run the server
    std::cout << "first run...." << std::endl; // debug
    my_sim.server_->Run(true, 200, false);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "second run...." << std::endl; // debug
    my_sim.server_->Run(true, 300, false);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "third run...." << std::endl; // debug
    my_sim.server_->Run(true, 300, false);

    my_sim.pause(); // quick test for me

    int num_creations = getForceCompCreation();

    std::cout << "here's num_creations" << num_creations << std::endl << std::flush;

    return 0;
}