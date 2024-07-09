#include "Simulator.h"
#define WORLD_IDX 0

#define STATE_SIZE 30
#define ACTION_SIZE 10



TrainSimulator::TrainSimulator(bool gui){
    
    axn_ = std::shared_ptr<double[]>(new double[ACTION_SIZE], std::default_delete<double[]>());
    state_ =std::shared_ptr<double[]>(new double[STATE_SIZE], std::default_delete<double[]>());
    
    for (int i = 0; i < ACTION_SIZE; i++){
        axn_[i] = 0.0;
    }
    
    for (int i = 0; i < STATE_SIZE; i++){
        state_[i] = 0.0; // state is defined by the LaTeX document
    }

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
    hasGUI = gui; // need this so destructor doesn't segfault in headless
    if (gui){
        // Spawn a new process with the GUI
        gui_ = std::make_unique<boost::process::child>(
            "ign gazebo -g -v4");

        bool guiServiceExists = false;
        ignition::transport::Node node;
        std::vector<std::string> serviceList;

        do {
            std::cout << "Waiting for GUI to show up... " << std::endl;
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

    // TODO: get robot's joint states and torso pose set.
    // based on gz-sim/examples
    


}

/*DESTRUCTIONNNNN*/
TrainSimulator::~TrainSimulator(){
    if (hasGUI){
        gui_->wait_for(std::chrono::seconds(5));
        ecm_ = nullptr;
        event_mgr_ = nullptr;
        
        if(gui_->running()){
            std::cerr << "TRAINSIMULATOR GUI FAILED TO EXIT WITHIN A TIMELY MANNER" << std::endl << std::flush;
            std::cerr << "Forcing shutdown of gui..."<< std::endl << std::flush;
            gui_->terminate();
            std::this_thread::sleep_for(std::chrono::seconds(5));
            gui_->wait();
            std::cerr << "Finished waiting on subprocess"<< std::endl << std::flush;
        }

        else{
            std::cout << "TrainSimulator GUI has successfully shut :)" << std::endl;
        }
    }
    server_->Stop();
}

/* start() */


/* step() 
* Given our inputAction
* step 1 ms in the simulation
*/
void TrainSimulator::step(std::vector<double> inputAction){
    // run our action
    {
        std::lock_guard<std::mutex> guard(axnMutex);
        for (int i = 0; i < 10; i++){
            axn_[i] = inputAction[i];
        }
        
    }
    bool stepped = server_->RunOnce(true); // true for running the simulation steps paused.
}    


/* stepFew()
* given our inputAction and numSteps
* step (numSteps) milliseconds in the 
* simulation with the given inputAction 
* put for the first frame of these steps.
*/
void TrainSimulator::stepFew(std::vector<double> inputAction, int numSteps){
    // run our action
    for (int i = 0; i < 10; i++){
        std::lock_guard<std::mutex> guard(axnMutex);
        axn_[i] = inputAction[i];
    }
    
    bool stepped = server_->Run(false, numSteps, false); // true for running the non-blocking.
}    


/* pause()
* pause the simulation
*/
void TrainSimulator::pause(){
    auto running = this->server_->Running();
    if (!running){
        std::cout << "TrainSimulator already paused. ignoring pause() call ..." << std::endl << std::flush;
        return;
    }

    // pause server
    bool hasPaused = this->server_->SetPaused(true, WORLD_IDX);
    if (!hasPaused){
        throw std::runtime_error("SetPaused() returned false...world does not exist?");
    }
}

/* unpause()
* unpause the simulation
*/

void TrainSimulator::unpause(){
    auto running = this->server_->Running();
    if (running){
        std::cout << "TrainSimulator already running. ignoring unpause() call ..." << std::endl << std::flush;
        return;
    }

    // upause server
    bool unPaused = this->server_->SetPaused(false, WORLD_IDX);
    if (!unPaused){
        throw std::runtime_error("SetPaused() returned false...world does not exist?");
    }

}


/* reset_simulation() */


