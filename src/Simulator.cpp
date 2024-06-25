#include "Simulator.h"
#include "process.hpp"


#define WORLD_IDX 0;

using namespace ignition;
using namespace gazebo;
using namespace systems;

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
        throw std::runtime_error("could not integrate ECMProvider into server");
    }
    /*
    TODO: gui code
    */
    if (gui){
        // Spawn a new process with the GUI
        gui_ = std::make_unique<TinyProcessLib::Process>(
            "ign gazebo -g -v 4");

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

    ecm_ = provider->ecm_ptr_;
    eventMgr_ = provider->event_mgr_ptr_;

    //TODO: get robot's joint states and torso pose set.

}

/* start() */


/* step() 
* step 1 ms in the simulation
*/
void TrainSimulator::step(vector<double> inputAction){
    for (int i = 0; i < 10; i++){
        axn_[i] = inputAction[i]
    }
}

/*stepFew()*/

/* pause()
* pause the simulation
*/


/* reset_simulation() */


