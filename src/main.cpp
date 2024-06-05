#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>

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

    bool hasBlackbird = server.HasEntity("blackbir",0);
    std::cout << "hasBlackbird: " << hasBlackbird << std::endl;

    // run the server
    server.Run(true, 0, false);


    return 0;
}