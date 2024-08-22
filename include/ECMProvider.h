#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <gz/sim/components/Name.hh>
#include <gz/sim/components/World.hh>
#include <memory>



/*ECMProvider.h

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
                        gz::sim::EntityComponentManager& ecm,
                        gz::sim::EventManager& eventMgr);
        void PostUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm);
        
        // getters and setters...or just getters
        const gz::sim::EntityComponentManager* getECM();
        const gz::sim::EventManager* getEvtMgr();

        
    
    private:
        std::shared_ptr<gz::sim::EntityComponentManager> ecm_; // not the owner
        std::shared_ptr<gz::sim::EventManager> evtmgr_; // not the owner
        std::string world_name_;

};

#endif