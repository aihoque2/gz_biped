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
    my_sim.server_->Run(true, 200, false);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    my_sim.server_->Run(true, 300, false);
    std::this_thread::sleep_for(std::chrono::seconds(3));

    my_sim.server_->Run(true, 300, false);
    // while(true){
    //     // listen to input for a button and during each button press, step environment
       
    // }

    return 0;
}