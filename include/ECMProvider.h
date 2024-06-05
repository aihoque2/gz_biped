#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <memory>


/*ECMProvider.h

lol idk if I need this but scenarIO has it
but I'll make my implementation more organized with bajillions more files.

UPDATE 06/04: I need this files to access utilize the server's EntityComponentManager
*/
#ifndef ECMPROVIDER_HPP
#define ECMPROVIDER_HPP

/*

*/
class ECMProvider: public ignition::gazebo::System, public ignition::gazebo::ISystemConfigure
{
    public:
        ECMProvider(); // create the ignition gazebo system
        void Configure(const ignition::gazebo::Entity& entity, 
        ignition::gazebo::EntityComponentManager& ecm,
        ignition::gazebo::EventManager& eventMgr);
    
    private:
        std::shared_ptr<ignition::gazebo::EntityComponentManager> ecm_ptr_;
        std::shared_ptr<ignition::gazebo::EntityComponentManager> eventMgr_ptr_;

};

#endif