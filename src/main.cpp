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

    TrainSimulator my_sim;
    bool hasBlackbird = my_sim->server_.HasEntity("blackbird", 0);
    std::cout << "hasBlackbird: " << hasBlackbird << std::endl;
    auto paused = my_sim->server_.Paused(); // need to auto bc it returns std::optional
    std::cout << "server paused: " << *paused << std::endl;

    // run the server
    my_sim->server_.Run(true, 0, false);

    return 0;
}