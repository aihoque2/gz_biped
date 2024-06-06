#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>
/*
NOTE: NO ROS2 to WORRY about here. only have to worry
aobout being able to pull full state out in valid manner

*/

using namespace ignition;
using namespace gazebo;
using namespace systems;

#ifndef TRAINSIMULATOR_H
#define TRAINSIMULATOR_H

/*Sim.h*/

class TrainSimulator{
    /*
    TODO:

    Hello, I am TrainSimulator
    you shall attach me to a gym
    environment wrapper via pybind11
    */

    public:
        TrainSimulator(); // constructor
        ~TrainSimulator(); // desctructor
        std::unique_ptr<gz::sim::Server> server_;
        std::unique_ptr<TInyProcessLib::Process> gui_; // note that gui of ignition simulation runs as a separate process.

        void pause();
        void step(double *action); // 10-array of joint commands to send each joint and step environment with joint action
        void reset_simulation(); // make action_cb ignore actions during the reset, then reset torque/velocities of joints and torso

        // "Simulation Resources"
        // TODO: should these be "smart pointers"? 
        std::shared_ptr<ignition::gazebo::EventManager> eventMgr_;
        std::shared_ptr<ignition::gazebo::EntityComponentManager> ecm_;

        // "PhsyicsData" not sure if needed
        double rtf = -1;
        double maxStepSize = -1;
        double realtimeUpdateRate= -1;

        bool valid_physics();
        bool spawn_blackbird();

    private:
        gz::sim::ServerConfig serverConfig;
        std::string sdfFile; // filename for the sdf to spawn
        std::string worldFile; // filename for the world

        std::shared_ptr<double[]> state_; // 1D 30-vector to represent our state
        std::shared_ptr<double[]> axn_; // 1D 10-vector to represent torques we send on each joint

        int STATE_SIZE; // should i be 

};

#endif