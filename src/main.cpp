#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>

int main(int argc, char **argv)
{

    // set verbosity to show los
    gz::common::Console::SetVerbosity(4);

    gz::sim::ServerConfig serverConfig;
    serverConfig.SetSdfFile("world/empty.world");

    gz::sim::Server server(serverConfig);

    server.Run(true /*blocking*/, 300 /*iterations*/, false /*paused*/);

    return 0;

}