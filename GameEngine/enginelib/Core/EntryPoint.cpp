#include <Core/Application.h>
#include <Core/EntryPoint.h>
#include <Core/PlatformDetection.h>
#include <Debug/Instrumentor.h>

// extern fts::Application* fts::Application::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char* argv[])
{
    fts::Log::Init();

    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = fts::Application::CreateApplication({argc, argv});
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
    HZ_PROFILE_END_SESSION();

    HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
    delete app;
    HZ_PROFILE_END_SESSION();
}

/*
#include <Core/Application.h>
#include <Core/PlatformDetection.h>
#include <Debug/Instrumentor.h>




//#ifdef FTS_PLATFORM_WINDOWS
extern fts::Application* fts::Application::CreateApplication(ApplicationCommandLineArgs args);


int main(int argc, char** argv)
{
    try
    {

        {

            fts::Log::Init();


         HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
         auto app = fts::Application::CreateApplication({ argc, argv });
         HZ_PROFILE_END_SESSION();

         HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
         app->Run();
         HZ_PROFILE_END_SESSION();

         HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
         delete app;
         HZ_PROFILE_END_SESSION();

        }
        return EXIT_SUCCESS;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Error in game: " << e.what() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
}

*/
