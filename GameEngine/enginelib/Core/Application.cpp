#include <Core/Application.h>
#include <Core/InputCore.h>
#include <Renderer/Gizmos.h>
#include <Renderer/GraphicsContext.h>
#include <Renderer/RenderCommand.h>
#include <Renderer/Renderer.h>
#include <Renderer/RendererAPI.h>
#include <Utils/FilePath.h>
#include <ftspch.h>

#ifdef FTS_PLATFORM_OSX
#include <cstdlib>
#include <libproc.h>
#endif

#include <SDL2/SDL.h>

namespace fts
{
    const std::string DEBUG_LOG_FILE   = "EngineDebugOutput.log";
    const std::string SETTING_FILENAME = "EngineSetting.ini";

    Application* Application::s_Instance = nullptr;

    Application::Application(ApplicationCommandLineArgs args, const std::string& title)
        : m_CommandLineArgs(args)
    {
        HZ_PROFILE_FUNCTION();

        mAppSpecification.WorkingDirectory = GetAppDirectory().string();
        mAppSpecification.CommandLineArgs  = m_CommandLineArgs;
        mAppSpecification.Name             = title;

        // const std::string debug_path = (GetAppDirectory() / DEBUG_LOG_FILE).generic_string();
        // fts::Log::Init(debug_path);

        FTS_ASSERT_MSG(!s_Instance, "Application already exists!");
        s_Instance = this;

        // FTS_CORE_INFO("Debug info is output to: {}", debug_path);

        // fts::Random::Init();
        // job::JobSystem::Initialize();
        // mThreadPool = CreateRef<job::ThreadPool>(job::ThreadPool::MAX_NUM_THREADS - 2);

        // fts::job::ThreadPoolLocator::provide(mThreadPool.get());

        InitSettings();

        auto wndProps = WindowCreateInfo(title);

        wndProps.Title  = title;
        wndProps.Width  = mSettings.GetInteger("window", "width", wndProps.Width);
        wndProps.Height = mSettings.GetInteger("window", "height", wndProps.Height);
        auto msaa       = mSettings.GetInteger("window", "msaa", static_cast<uint32_t>(wndProps.MSAA));
        wndProps.MSAA   = static_cast<MultiSampleLevel>(msaa);
        wndProps.VSync  = mSettings.GetInteger("window", "vsync", wndProps.VSync);

        CreateWindowApp(wndProps);
        mSettingApp = wndProps;

        m_Window->GetGraphicsContext()->MakeCurrentContext();

        InitEvents();

        InputCore::Init();
        {
            Gizmos::create(10000, 10000, 10000, 10000);

            RenderCommand::Init();

            // ResourceManager::Initialize();
            Renderer::Init(wndProps.Width, wndProps.Height);
        }

        // mScene = CreateRef<fts::Scene>();
        // fts::Scene::set_active_scene(*mScene.get());
        // mActiveScene = mScene;

        // PushLayer<ConsoleLayer>();
        // m_ImGuiLayer = PushLayer<ImGuiLayer>();
    }

    Application::~Application()
    {
        HZ_PROFILE_FUNCTION();

        auto& application = Get();
        // application.mActiveScene.reset();

        Renderer::Shutdown();
        Gizmos::destroy();

        // ResourceManager::Shutdown();

        // mScene.reset();
        // mScene = nullptr;

        for(auto& i : application.m_LayerStack)
        {
            i.reset();
            i = nullptr;
        }

        application.m_LayerStack.clear();

        // m_ImGuiLayer.reset();
        //
        // delete mRendererDevice;
        // mRendererDevice = nullptr;

        // mCommandBucket->Clear();
        // std::shared_ptr<CommandBucket> commandBuffer = mCommandBucket;
        // mCommandBucket.reset();

        // std::shared_ptr<DeferredCommandBuffer> deferredCommandBuffer = mDeferredCommandBuffer;
        // deferredCommandBuffer.reset();

        if(m_Window)
        {
            mSettings.SetInteger("window", "width", m_Window->GetWidth());
            mSettings.SetInteger("window", "height", m_Window->GetHeight());
            mSettings.SetInteger("window", "msaa", static_cast<int>(m_Window->GetMSAA()));
            mSettings.SetBoolean("window", "vsync", m_Window->IsVSync());

            mSettings.Save((GetAppDirectory() / SETTING_FILENAME).string());

            delete m_Window;
            m_Window = nullptr;
        }

        Log::Uninitialize();
    }

    void Application::CreateWindowApp(const WindowCreateInfo& props)
    {
        m_Window = fts::Window::Create(props);
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::HandleEvent));

        // auto iconPath = fts::getPathRelativeRoot("Icons/light.png");
        // m_Window->SetIcon(iconPath);
    }

    std::filesystem::path Application::GetAppDirectory()
    {
        static std::filesystem::path path;
        if(path.empty())
        {
            char buffer[255];
#if FTS_PLATFORM_LINUX
            std::uint32_t size = readlink("/proc/self/exe", buffer, sizeof(buffer));
#elif FTS_PLATFORM_WINDOWS
            std::uint32_t size = GetModuleFileNameA(NULL, buffer, sizeof(path));

#elif FTS_PLATFORM_OSX

            pid_t pid          = getpid();
            std::uint32_t size = proc_pidpath(pid, buffer, sizeof(buffer));
            if(size <= 0)
            {
                FTS_CORE_ERROR("{} readlink(\"/proc/self/exe\", ...)  failed", "GetExecutablePath");
            }

#endif
            if(size > 0)
            {
                path.assign(std::begin(buffer), std::begin(buffer) + size);
            }
        }
        return path.parent_path();
    }

    void Application::InitSettings()
    {
        std::filesystem::path ini_path = GetAppDirectory() / SETTING_FILENAME;

        FTS_CORE_INFO("Ini path is output to: {}", ini_path.string());

        if(std::filesystem::exists(ini_path))
        {
            mSettings.Load(ini_path.string());
        }
        else
        {
            FTS_CORE_WARN("No such ini file: {}. The application will create a new one.", ini_path.string());
        }
    }

    void Application::InitEvents()
    {

        // mAppListener.RegisterHandler(FTS_EVENT_KEY, [&](const Event& evt)
        //                              {
        //                                  auto keyEvent = evt.get<KeyEvent>();
        //                                  if(keyEvent.type == fts::KeyEvent::Type::Down)
        //                                  {
        //                                      switch(keyEvent.key)
        //                                      {
        //                                      case SDL_SCANCODE_F2:
        //                                          m_Window->TakeScreenshot();
        //                                          break;
        //                                      }
        //                                  }
        //                              });

        mAppListener.RegisterHandler(FTS_EVENT_KEY_PRESSED, this, &Application::OnKeyPressedEvent);

        mAppListener.RegisterHandler(FTS_EVENT_QUIT, this, &Application::OnWindowClose);
        mAppListener.RegisterHandler(FTS_EVENT_WINDOW_CLOSE, this, &Application::OnWindowClose);
        mAppListener.RegisterHandler(FTS_EVENT_WINDOW_RESIZE, this, &Application::OnWindowResize);
        mAppListener.RegisterHandler(FTS_EVENT_WINDOW_MINIMIZED, this, &Application::OnWindowMinimized);
        mAppListener.RegisterHandler(FTS_EVENT_WINDOW_RESTORE, this, &Application::OnWindowRestored);
    }

    /*
    std::shared_ptr<fts::Scene> Application::GetActiveScene()
    {
        auto& application = Get();
        return application.mActiveScene;
    }

    void Application::Attach(const std::shared_ptr<fts::Scene>& scene)
    {

        auto& application        = Get();
        application.mActiveScene = scene;
        for(auto& func : application.mPostAttachSceneFunctions)
        {
            func(scene);
        }

        for(auto& layer : application.m_LayerStack)
        {
            layer->mScene = scene;
        }
    } */

    void Application::Run()
    {
        HZ_PROFILE_FUNCTION();

        mApplicationStatus = ApplicationStatus::Uninitialized;

        // Initializes the client application, handles registering all the application layers
        Initialize();

        // Initialize the timers to default
        float fixedStep = 0.0f;
        float frameStep = 0.0f;

        // Perform initializations for all application layers
        for(auto& layer : m_LayerStack)
        {
            layer->OnAttach();
        }

        mApplicationStatus = ApplicationStatus::Initialized;

        while(m_Running)
        {
            HZ_PROFILE_SCOPE("RunLoop");

            /* Process input events */
            {
                HZ_PROFILE_SCOPE("sdlPollEvents");
                InputCore::Update();

                // Poll for window events
                m_Window->ProcessEvents();

                mAppListener.HandleAllEvents();
            }

            float time        = static_cast<float>(SDL_GetTicks()); // Time::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime   = time;

            if(!m_Minimized)
            {

                {
                    HZ_PROFILE_SCOPE("LayerStack OnUpdate");
                    // Update all the application layers (variable rate updates, run as fast as possible)
                    for(auto& layer : m_LayerStack)
                    {
                        if(layer->IsEnabled)
                            layer->OnUpdate(timestep);
                    }
                }

                {

                    HZ_PROFILE_SCOPE("LayerStack Onrenderer");
                    // Perform the render step if enough time has passed
                    // if (frameStep > Timing::FrameTimeStep)
                    {
                        RenderBegin();
                        {

                            // Pre-render for all layers
                            for(auto& layer : m_LayerStack)
                            {
                                if(layer->IsEnabled)
                                    layer->OnPreRender();
                            }

                            // Render all the application layers
                            for(auto& layer : m_LayerStack)
                            {
                                if(layer->IsEnabled)
                                    layer->OnRenderer(timestep);
                            }

                            // Post-render for all layers
                            for(auto& layer : m_LayerStack)
                            {
                                if(layer->IsEnabled)
                                    layer->OnPostRender();
                            }

#ifdef FTS_IMGUI

                            m_ImGuiLayer->Begin();
                            {
                                HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

                                for(auto& layer : m_LayerStack)
                                    if(layer->IsEnabled)
                                        layer->OnImGuiRender();
                            }
                            m_ImGuiLayer->End();
#endif
                        }
                        RenderEnd();
                    }
                }
            }
        }

        // Perform the shutdown behaviors for all application layers
        for(auto& layer : m_LayerStack)
        {
            layer->OnDetach();
        }

        // Shuts down the client application, allowing client apps to provide shutdown behavior
        Shutdown();
    }

    void Application::RenderBegin()
    {
        m_Window->BeginFrame();
    }

    void Application::RenderEnd()
    {
        m_Window->EndFrame();
    }

    void Application::Render()
    {

        {
            HZ_PROFILE_SCOPE("LayerStack OnRenderer");

            for(auto& layer : m_LayerStack)
                layer->OnRenderer(m_DeltaTime);
        }

#ifdef FTS_IMGUI
        m_ImGuiLayer->Begin();
        {
            HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");

            for(auto& layer : m_LayerStack)
                layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();
#endif
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::HandleEvent(const Event& evt)
    {

        for(auto& layer : m_LayerStack)
            layer->OnEvent(evt);
    }

    void Application::OnKeyPressedEvent(const Event& evt)
    {

        const auto& key = evt.get<KeyPressedEvent>();
        if(key.Repeat > 0)
        {
            return;
        }
        switch(key.Key)
        {
        case fts::Key::KeyCode::KEY_F2:
        {
            int i = 0;
        }
        }

        if(key.Key == fts::Key::KeyCode::KEY_F2)
        {
            // m_Window->TakeScreenshot();
        }
    }

    void Application::OnWindowClose(const Event& evt)
    {
        Close();
    }

    void Application::OnWindowResize(const Event& evt)
    {
        auto data = evt.get<WindowResizeEvent>();

        if(data.Width == 0 || data.Height == 0)
        {
            m_Minimized = true;
        }

        m_Minimized = false;

        for(auto& layer : m_LayerStack)
            if(layer->IsEnabled)
                layer->OnWindowResize(data.Width, data.Height);

        Renderer::OnWindowResize(data.Width, data.Height);
    }

    void Application::OnWindowMinimized(const Event& evt)
    {
        m_Minimized = true;
    }

    void Application::OnWindowRestored(const Event& evt)
    {
        m_Minimized = false;
    }

} // namespace fts
