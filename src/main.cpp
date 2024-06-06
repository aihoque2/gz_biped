#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <gz/sim/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>
#include <iostream>

// class ModelNameProvider: public {
    
// };

int main(int argc, char **argv)
{
    // debug
    gz::common::Console::SetVerbosity(4);

    // Initialize Ignition Gazebo
    gz::sim::ServerConfig serverConfig;
    serverConfig.SetSdfFile("world/empty.world");

    // No longer needed bc blackbird is already in world file
    //std::string blackbirdFile = "urdf/blackbird.sdf" 

    gz::sim::Server server(serverConfig);

    bool hasBlackbird = server.HasEntity("blackbird", 0);
    std::cout << "hasBlackbird: " << hasBlackbird << std::endl;
    auto paused = server.Paused(); // need to auto bc it returns std::optional
    std::cout << "server paused: " << *paused << std::endl;

    // run the server
    server.Run(true, 0, false);

    return 0;
}