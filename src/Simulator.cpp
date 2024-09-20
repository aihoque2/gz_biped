#include "Simulator.h"

TrainSimulator::TrainSimulator(bool gui){
    
    axn_ = std::shared_ptr<double[]>(new double[ACTION_SIZE], std::default_delete<double[]>());
    state_ = std::shared_ptr<double[]>(new double[STATE_SIZE], std::default_delete<double[]>());
    contacted_ = std::shared_ptr<bool[]>(new bool[3], std::default_delete<bool[]>());
    
    for (int i = 0; i < ACTION_SIZE; i++){
        axn_[i] = 0.0;
    }
    
    for (int i = 0; i < STATE_SIZE; i++){
        state_[i] = 0.0; // state is defined by the LaTeX document
    }

    worldFile = "world/empty.world";
    gz::common::Console::SetVerbosity(4);
    serverConfig.SetSdfFile(worldFile); //member var
    
    server_ = std::make_unique<gz::sim::Server>(serverConfig);

    // adding ECMProvider idx
    provider_ = std::make_shared<ECMProvider>();
    const auto gotECM = server_->AddSystem(provider_, WORLD_IDX);
    if (!gotECM){
        throw std::runtime_error("could not integrate ECMProvider into server");
    }

    // EffortController plugin
    auto controller = std::make_shared<EffortController>(axnMutex, axn_);
    const auto gotCtrl = server_->AddSystem(controller, WORLD_IDX);
    if (!gotCtrl){
        throw std::runtime_error("could not integrate EffortController into server");
    }
    
    // TODO: Contact Sensor plugin
    auto contact_sensor = std::make_shared<BipedalContact>(contactMutex, contacted_);
    const auto gotContact = server_->AddSystem(contact_sensor, WORLD_IDX);
    if (!gotContact){
        throw std::runtime_error("could not integrate BipedalContact into server");
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

    ecm_ = provider_->getECM();
    event_mgr_ = provider_->getEvtMgr();

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
            KillPIDs("gz\\ sim\\");

        }
    }
    std::cout << "Stopping TrainSimulator server..." << std::endl;
    server_->Stop();
}

/* KillPIDs() 
* kill gz_sim processes at the end of 
*/
void TrainSimulator::KillPIDs(std::string process_name){
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

/* Step() 
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
*
* I made this function because this
* simulation
* is for force control.
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


/* pause()

* pause the simulation
*/
void TrainSimulator::Pause(){
    auto running = this->server_->Running();
    if (!running){
        std::cout << "TrainSimulator already paused. ignoring pause() call ..." << std::endl;
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
        return;
    }

    // upause server
    bool unPaused = this->server_->SetPaused(false, WORLD_IDX);
    if (!unPaused){
        throw std::runtime_error("SetPaused() returned false...world does not exist?");
    }

}

void TrainSimulator::ResetSim(){

    ecm_ = provider_->getECM(); // just to be safe

    /*
    std::vector<gz::sim::Entity> jointVec = ecm_->EntitiesByComponents(gz::sim::components::Joint());
    std::cout << "ResetSim() sanity check jointVec size: " << jointVec.size() << std::endl;
    */

    gz::sim::Entity blackbird_ent = ecm_->EntityByComponents(gz::sim::components::Name("blackbird"));
    
    if (blackbird_ent == gz::sim::kNullEntity){
        throw std::runtime_error("TrainSimulator could not find the blackbird entity...wtf");
    }
    
    auto* cl_pose_comp = ecm_->Component<gz::sim::components::Pose>(blackbird_ent);
    if (cl_pose_comp == nullptr){
        throw std::runtime_error("TrainSimulator blackbird_ent's Pose component is NULL");
    }

    // TODO
    gz::math::Pose3d initial_pose(0.0, 0.0, 1.15, 0.0, 0.0, 0.0);
    // *cl_pose_comp = gz::sim::components::Pose(initial_pose);
    
    auto* pose_cmd = ecm_->Component<gz::sim::components::Pose>(blackbird_ent);
    if (pose_cmd == nullptr){
        ecm_->CreateComponent(blackbird_ent, gz::sim::components::WorldPoseCmd(initial_pose));
    }
    else{
        // set the components pose
        ecm_->SetComponentData<gz::sim::components::WorldPoseCmd>(blackbird_ent, initial_pose);
    }

    ecm_->SetChanged(blackbird_ent,
        gz::sim::components::WorldPoseCmd::typeId, 
        gz::sim::ComponentState::OneTimeChange);
    
    /*TODO: is this part necessary or delete it?*/
    // gz::math::Vector3d initial_vel(0.0, 0.0, 0.0);
    // auto* lin_vel_cmd = ecm_->Component<gz::sim::components::LinearVelocityCmd>(blackbird_ent);
    // if (lin_vel_cmd == nullptr){
    //     ecm_->CreateComponent(blackbird_ent, gz::sim::components::LinearVelocityCmd(initial_vel));
    // }
    // else{
    //     ecm_->SetComponentData<gz::sim::components::LinearVelocityCmd>(blackbird_ent, initial_vel);
    // }

    // ecm_->SetChanged(blackbird_ent,
    //     gz::sim::components::LinearVelocityCmd::typeId, 
    //     gz::sim::ComponentState::OneTimeChange);


    for (auto joint_name : JOINT_NAMES){
        auto joint = ecm_->EntityByComponents(gz::sim::components::Joint(), 
                                                gz::sim::components::Name(joint_name));
        /* TODO: reset each joint state */ 

        // theta reset (position)
        auto * joint_reset = ecm_->Component<gz::sim::components::JointPositionReset>(joint);
        
        if (joint_reset == nullptr)
            ecm_->CreateComponent(joint, gz::sim::components::JointPositionReset({0.0}));

        else 
            ecm_->SetComponentData<gz::sim::components::JointPositionReset>(joint, {0.0});
        

        // theta-dot
        auto* joint_vel = ecm_->Component<gz::sim::components::JointVelocityReset>(joint);
        
        if (joint_vel == nullptr){
            ecm_->CreateComponent(blackbird_ent, gz::sim::components::JointVelocityReset({0.0}));
        }
        else{
            // set the components pose
            ecm_->SetComponentData<gz::sim::components::JointVelocityReset>(blackbird_ent, {0.0});
        }

        // theta-double-dot???
        auto* force = ecm_->Component<gz::sim::components::JointForceCmd>(joint);
        force->Data()[0] = 0.0;

    }

}

