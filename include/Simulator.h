#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>

using namespace ignition;
using namespace gazebo;
using namespace systems;

#ifndef RESET_SIMULATION_H
#define RESET_SIMULATION_H

/*Sim.h*/
struct PhysicsData{

}

class Simulator{

    public:
        Simulator();
        std::unique_ptr<gz::sim::Server> server;
        std::unique_ptr<TInyProcessLib::Process> gui;
        ignition::gazebo::System;

    private:
        gz::sim::ServerConfig serverConfig;


};

ignition::msgs::
#endif