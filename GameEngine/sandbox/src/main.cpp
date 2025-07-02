#include <Core/EntryPoint.h>
#include <ftscore.h>

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

            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

RegisterApplication(example01::SandboxApp);
