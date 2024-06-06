#include "include/Simulator.h"
#include "include/ECMProvider.h"

#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>

#define WORLD_IDX 0;

TrainSimulator::TrainSimulator(bool gui){
    
    // debug
    gz::common::Console::SetVerbosity(4);
    gz::sim::serverConfig serverConfig; 
    serverConfig.SetSdfFile("world/empty.world");

    server_ = std::make_unique<gz::sim::Server>(serverConfig);

    // adding world idx
    auto provider = std::make_shared<ECMProvider>();
    const auto ok = server_->AddSystem(provider, WORLD_IDX);
    if (!ok){
        throw run
    }
    /*
    TODO: gui code
    */
    if (gui){
        // Spawn a new process with the GUI
        gui_ = std::make_unique<TinyProcessLib::Process>(
            "ign gazebo -g -v " + std::to_string(guiVerbosity) + redirect);

        bool guiServiceExists = false;
        ignition::transport::Node node;
        std::vector<std::string> serviceList;

        do {
            sDebug << "Waiting GUI to show up... " << std::endl;
            node.ServiceList(serviceList);

            for (const auto& serviceName : serviceList) {
                if (serviceName.find("/gui/") == 0) {
                    guiServiceExists = true;
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (!guiServiceExists);
        // end gui bracket
    } // endif

}

/* start() */
void TrainSimulator::start(){
    
}


/* step() 
* step 1 ms in the simulation
*/
void TrainSimulator::step(){}

/* pause()
* pause the simulation
*/


/* reset_simulation() */


