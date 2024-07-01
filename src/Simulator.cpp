#include "Simulator.h"
#define WORLD_IDX 0

using namespace ignition;
using namespace gazebo;
using namespace systems;

TrainSimulator::TrainSimulator(bool gui){
    
    // debug
    gz::common::Console::SetVerbosity(4);
    gz::sim::ServerConfig config; 
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
        gui_ = std::make_unique<boost::process::child>(
            "ign gazebo -g -v 4");

        bool guiServiceExists = false;
        ignition::transport::Node node;
        std::vector<std::string> serviceList;

        do {
            std::cout << "Waiting GUI to show up... " << std::endl;
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

    ecm_ = provider->getECM();
    event_mgr_ = provider->getEvtMgr();

    //TODO: get robot's joint states and torso pose set.

}


/*DESTRUCTIONNNNN*/
TrainSimulator::~TrainSimulator(){

    gui_->wait_for(std::chrono::seconds(5));
    
    if(gui_->running()){
        std::cerr << "TRAINSIMULATOR GUI FAILED TO EXIT WITHIN A TIMELY MANNER" << std::endl << std::flush;
        std::cerr << "Forcing shutdown of gui..."<< std::endl << std::flush;
        gui_->terminate();
        std::this_thread::sleep_for(std::chrono::seconds(5));
        gui_->wait();

    }

    else{
        std::cout << "TrainSimulator has successfully shut :)" << std::endl;
    }
}

/* start() */


/* step() 
* step 1 ms in the simulation
*/
void TrainSimulator::step(std::vector<double> inputAction){
    // run our action
    for (int i = 0; i < 10; i++){
        std::lock_guard<std::mutex> guard(axnMutex);
        axn_[i] = inputAction[i];
    }

    bool stepped = server_->RunOnce(true); // true for running the simulation steps paused.
}

/*stepFew()*/

/* pause()
* pause the simulation
*/


/* reset_simulation() */


