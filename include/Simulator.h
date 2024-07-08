
// local includes
#include "ECMProvider.h"

// higher lvl includes
#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/components/Name.hh>
#include <ignition/gazebo/components/Joint.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <ignition/transport/Node.hh>
#include <boost/process.hpp>
#include <memory>

/*
NOTE: NO ROS2 to WORRY about here. only have to worry
aobout being able to pull full state out in valid manner

!!!the ROS2 nodes to step and publish will be written with rclpy!!!
*/

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

        /*
        functions
        */
        TrainSimulator(bool gui); // constructor
        ~TrainSimulator(); // desctructor

        // should this be void, or should this return state values?
        void step(std::vector<double> inputAction); // 10-array of joint commands to send each joint and step environment with joint action
        void stepFew(std::vector<double> inputAction, int numSteps);

        void set_action(double *action); // mainly a helper for step() to make modifying axn_ thread-safe       
        void run(bool train); // just run the simulation like a main example...we'll figure out the rest later

        void pause();
        void unpause();

        /*NOTE: DO THIS AFTER MAKING A JOINT CONTROLLER*/
        void reset_simulation(); // make action_cb ignore actions during the reset, then reset torque/velocities of joints and torso

        /*
        variables 
        */
        std::unique_ptr<gz::sim::Server> server_;
        std::unique_ptr<boost::process::child> gui_; // note that gui of ignition simulation runs as a separate process.

        // "Simulation Resources"
        const ignition::gazebo::EventManager* event_mgr_; // not the owner, so raw ptr OK
        const ignition::gazebo::EntityComponentManager* ecm_; // not the owner, so raw ptr OK

        // "PhsyicsData" not sure if needed
        double rtf = -1;
        double maxStepSize = -1;
        double realtimeUpdateRate= -1;

    private:
        gz::sim::ServerConfig serverConfig;
        std::string sdfFile; // filename for the sdf to spawn
        std::string worldFile; // filename for the world

        std::shared_ptr<double[]> state_; // 1D 30-vector to represent our state
        std::shared_ptr<double[]> axn_; // 1D 10-vector to represent torques we send on each joint
        
        std::mutex axnMutex; // lock used for set_action() and get_action()
        std::mutex stateMutex; // lock used for publishing state info

        bool hasGUI;

};

#endif