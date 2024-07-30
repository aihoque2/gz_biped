/*
ContactSensor.h

ContactSensor system to manage the blackbird's contacts

TODO: REGISTER THIS PLUGIN TO BE ABLE TO USE IN SDF FILES
*/

#include <memory>


class BipedalContact : public gz::sim::System,
                        public gz::sim::ISystemConfigure,
                        public gz::sim::ISystemPostUpdate
{
    public:
        BipedalContact(std::mutex& contactMutex, std::shared_ptr<bool[]> contacted_);
        ~BipedalContact() final = default;

        void CreateSensors();
        void UpdateSensors();
        

        // inherited from ISystemConfigure ABC
        void Configure(const ignition::gazebo::Entity& entity,
                        const std::shared_ptr<const sdf::Element>&, //doc-inherited
                        ignition::gazebo::EntityComponentManager& ecm,
                        ignition::gazebo::EventManager& eventMgr);

        // inherited from ISystemPreUpdate ABC
        void PreUpdate(const gz::sim::UpdateInfo& info,
                        gz::sim::EntityComponentManager& ecm);


    private:
        std::shared_ptr<bool[]> contacted; // footL, footR, Body
        std::mutex& contact_mutex_; // thread-safe lezzgeddit
}