#include "Simulator.h"

TrainSimulator::TrainSimulator(bool gui){
    
    axn_ = std::shared_ptr<double[]>(new double[ACTION_SIZE], std::default_delete<double[]>());
    state_ = std::shared_ptr<double[]>(new double[STATE_SIZE], std::default_delete<double[]>());
    
    for (int i = 0; i < ACTION_SIZE; i++){
        axn_[i] = 0.0;
    }
    
    for (int i = 0; i < STATE_SIZE; i++){
        state_[i] = 0.0; // state is defined by the LaTeX document
    }

    // debug
    worldFile = "world/empty.world";
    gz::common::Console::SetVerbosity(4);
    serverConfig.SetSdfFile(worldFile); //member var
    
    server_ = std::make_unique<gz::sim::Server>(serverConfig);

    // adding ECMProvider idx
    auto provider = std::make_shared<ECMProvider>();
    const auto gotECM = server_->AddSystem(provider, WORLD_IDX);
    if (!gotECM){
        throw std::runtime_error("could not integrate ECMProvider into server");
    }

    // EffortController plugin
    auto controller = std::make_shared<EffortController>(axnMutex, axn_);
    const auto gotCtrl = server_->AddSystem(controller, WORLD_IDX);
    if (!gotCtrl){
        throw std::runtime_error("could not integrate EffortController into server");
    }
    

    /*
    gui code
    */
    hasGUI = gui; // need this so destructor doesn't segfault in headless
    if (gui){
        // Spawn a new process with the GUI
        gui_ = std::make_unique<boost::process::child>(
            "gz sim world/empty.world -v 4");

        bool guiServiceExists = false;
        std::unique_ptr<gz::transport::Node> node =  std::make_unique<gz::transport::Node>();
        std::vector<std::string> serviceList;

        do {
            std::cout << "Waiting for GUI to show up... " << std::endl;
            node->ServiceList(serviceList);

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

}


/*DESTRUCTIONNNNN*/
TrainSimulator::~TrainSimulator(){
    ecm_ = nullptr;
    event_mgr_ = nullptr;
        
    if (hasGUI && gui_){

        if(gui_->running()){
            std::cerr << "TRAINSIMULATOR GUI FAILED TO EXIT WITHIN A TIMELY MANNER" << std::endl << std::flush;
            std::cerr << "Forcing shutdown of gui..."<< std::endl << std::flush;
            gui_->terminate();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            gui_->wait();
            std::cerr << "Finished waiting on subprocess"<< std::endl << std::flush;
        }
    }
    std::cout << "Stopping TrainSimulator server..." << std::endl;
    server_->Stop();

    KillProcessIDs("gz\\ sim\\ ");

}

/* KillProcessIDs() 
* kill gz_sim processes at the end of 
*/
void TrainSimulator::KillProcessIDs(std::string process_name){
    std::array<char, 128> buffer;
    std::vector<std::string> pids;
    std::string command = "ps aux | grep " + process_name + " ";

    const char* cmd = command.c_str();
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe){
        throw std::runtime_error("[ERROR] GetProcessIDs() in TrinSimulator.cpp: popen() failed!");
    }

    // red the output and get our data
    while(fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr){
        const std::string result = buffer.data();

        std::istringstream iss(result);
        std::string token;
        int token_count = 0;
        while (iss >> token){
            if (token_count == 1){ // idx 1 gives the PID
                int pid = std::stoi(token);
                kill(pid, SIGTERM);
                break; // next loop iter
            }
            token_count++;
        }

        pids.push_back(result);

    }

} 

/* step() 
* Given our inputAction
* step 1 ms in the simulation
*/
void TrainSimulator::Step(std::vector<double> inputAction){
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
void TrainSimulator::StepFew(std::vector<double> inputAction, int axnSteps, int afterSteps){
    bool stepped = false;
    
    // run our action steps
    for (int i = 0; i < axnSteps; i++){    
        // set our action and lock our mutex within scope
        {
            std::lock_guard<std::mutex> guard(axnMutex);        
            for (int k = 0; k < 10; ++k){
                axn_[k] = inputAction[k];
            }
        }
        stepped = server_->RunOnce(false);
    }

    // run our normal steps
    stepped = server_->Run(false, afterSteps, false); // true for running the non-blocking.
}    


/* pause()        int getForceCompCreation();

* pause the simulation
*/
void TrainSimulator::Pause(){
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

void TrainSimulator::Unpause(){
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