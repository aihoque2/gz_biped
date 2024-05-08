#include <gz/sim/Server.hh>
#include <gz/common/Console.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/rendering.hh>
#include <ignition/gui.hh>
#include <ignition/common.hh>

int main(int argc, char **argv)
{
    // debug
    gz::common::Console::SetVerbosity(4);

    // Initialize Ignition Gazebo
    gz::sim::ServerConfig serverConfig;
    serverConfig.SetSdfFile("shapes.world");

    gz::sim::Server server(serverConfig);

    // Initialize Ignition GUI and Rendering
    ignition::gui::Application app(argc, argv);
    app.LoadConfig("gui.config");

    // Get engine and scene
    auto engine = ignition::rendering::engine("ogre2");
    if (!engine)
    {
        std::cerr << "Engine 'ogre2' is not available." << std::endl;
        return 1;
    }

    // Create a scene
    auto scene = engine->CreateScene("MyScene");
    if (!scene)
    {
        std::cerr << "Failed to create scene." << std::endl;
        return 1;
    }

    // Configure scene properties as needed
    scene->SetAmbientLight(0.8, 0.8, 0.8);

    // Initialize Ignition Gazebo server with a configuration
    serverConfig.SetSdfFile("shapes.sdf");


    // Run the server
    server.Run(true, 0, false);

    // Clean up and destroy scene
    engine->DestroyScene(scene);

    return 0;
}