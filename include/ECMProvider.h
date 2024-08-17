#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/World.hh>
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
class ECMProvider: public gz::sim::System, public gz::sim::ISystemConfigure
{
    public:
        ECMProvider(); // create the ignition gazebo system
        ~ECMProvider();
        void Configure(const gz::sim::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, 
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr);

        
        // getters and setters...or just getters
        const ignition::gazebo::EntityComponentManager* getECM();
        const ignition::gazebo::EventManager* getEvtMgr();

        
    
    private:
        const ignition::gazebo::EntityComponentManager* ecm_ptr_; // not the owner
        const ignition::gazebo::EventManager* evtmgr_ptr_; // not the owner
        std::string world_name_;

};

#endif