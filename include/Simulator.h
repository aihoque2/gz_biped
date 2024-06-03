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
        std::unique_ptr<gz::sim::Server> server_;
        std::unique_ptr<TInyProcessLib::Process> gui_; // note that gui of ignition simulation runs as a separate process.

        void start();
        void pause();
        void step();
        void reset_simulation(); // make action_cb ignore action

        // "Simulation Resources"
        // TODO: should these be "smart pointers"? 
        ignition::gazebo::EventManager* eventMgr = NULL;
        ignition::gazebo::EntityComponentManager* ecm = NULL;

        // "PhsyicsData"
        double rtf = -1;
        double maxStepSize = ;
        double realtimeUpdateRate= -1;

        bool valid_physics()

    private:
        gz::sim::ServerConfig serverConfig;
        std::string sdfFile; // filename for the sdf to spawn


};

#endif