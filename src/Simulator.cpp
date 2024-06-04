#include "include/Simulator.h"

TrainSimulator::TrainSimulator(bool gui){
    server_ = std::make_unique<gz::sim::Server>();


    if (gui){
        /*
        TODO: gui code
        */

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
    }
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


