#include <memory>

#ifndef WORLD_H
#define WORLD_H

class World{
    public:
        bool insertModel()

    private:
        std::shared_ptr<ignition::gazebo::SdfEntityCreator> sdfEntityCreator;
};

#endif