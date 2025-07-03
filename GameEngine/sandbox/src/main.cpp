#include <Core/EntryPoint.h>
#include <SDL.h>
#include <Utils/IncludeGL.h>
#include <ftscore.h>
#include <iostream>

namespace example01
{

    class RenderLayer : public fts::Layer
    {
    public:
        RenderLayer(const std::string& n)
            : fts::Layer(n)
        {
        }
        ~RenderLayer()
        {
        }

        virtual void OnAttach()
        {
        }
        virtual void OnDetach()
        {
        }

        virtual void OnLateUpdate(const fts::Timestep& ts) {};
        virtual void OnUpdate(const fts::Timestep& ts) {};
        virtual void OnFixedUpdate(const fts::Timestep& ts) {};

        virtual void OnPreRender()
        {

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // fts::RenderCommand::SetClearColor({ 0.32f, 0.3185f, 0.32f, 1.0f });
            // fts::RenderCommand::Clear();
            // fts::RenderCommand::SetDepthTest(true);
        }

        virtual void OnRenderer(const fts::Timestep& ts)
        {
        }

        virtual void OnPostRender() {};

        void OnEvent(const fts::Event& event)
        {
        }

        void OnWindowResize(uint32_t width, uint32_t height)
        {
            // mProjection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
        }
    };

    class SandboxLayer : public fts::Layer
    {

    private:
        uint32_t mWidth  = 0;
        uint32_t mHeight = 0;

    public:
        SandboxLayer(const std::string& n)
            : fts::Layer(n)
        {
        }
        ~SandboxLayer()
        {
        }

        void OnAttach()
        {
        }

        void OnDetach()
        {
        }

        void OnUpdate(const fts::Timestep& ts)
        {
        }
    };

    class SandboxApp : public fts::Application
    {
    public:
        SandboxApp(fts::ApplicationCommandLineArgs args)
            : fts::Application(args, "Command Bucket 3D Layer") {
                //   PushLayer<SceneBuilder>( ("Scene Builder"));
                //   PushLayer<SceneBuilder>(("SandboxLayer Test"));
                // PushLayer<> ( RenderLayer("RenderLayer Test"));
            };

        void Initialize()
        {
        }

        void Shutdown()
        {
        }
    };

} // namespace example01

// RegisterApplication(example01::SandboxApp);

int main(int argc, char* argv[])
{
    try
    {

        {
            fts::Log::Init();

            HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
            // auto app = fts::Application::CreateApplication({argc, argv});

            auto app = new fts::Application({argc, argv});
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
    catch(std::runtime_error& e)
    {
        std::cerr << "Error in game: " << e.what() << std::endl;
        system("pause");
        return EXIT_FAILURE;
    }
}
