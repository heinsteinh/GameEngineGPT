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

        const std::string debug_path = (GetAppDirectory() / DEBUG_LOG_FILE).generic_string();
        fts::Log::Init(debug_path);

        FTS_ASSERT_MSG(!s_Instance, "Application already exists!");
        s_Instance = this;

        FTS_CORE_INFO("Debug info is output to: {}", debug_path);

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

        auto iconPath = fts::getPathRelativeRoot("Icons/light.png");
        m_Window->SetIcon(iconPath);
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
                FTS_CORE_ERROR("GetExecutablePath", "readlink(\"/proc/self/exe\", ...)  failed");
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
} // namespace fts
