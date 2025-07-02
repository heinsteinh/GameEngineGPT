
#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <Core/Assert.h>
#include <Core/IWindow.h>
#include <Core/Ini.h>
#include <Core/Layer.h>
#include <Core/Log.h>
#include <Events/Event.h>
#include <Events/EventDef.h>
#include <filesystem>

namespace fts
{

    struct ApplicationCommandLineArgs
    {
        int Count   = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            FTS_ASSERT(index < Count);
            return Args[index];
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "GTZ Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };

    enum class ApplicationStatus
    {
        Uninitialized,
        Initialized,
        OnDestroy
    };

    enum class GameStatus
    {
        Stop,
        Pause,
        Step,
        Playing
    };

    class Application
    {
    public:
        Application(const std::string& name         = "SGL App",
                    ApplicationCommandLineArgs args = ApplicationCommandLineArgs());

        Application() = default;

        virtual ~Application();

        static Application* CreateApplication(fts::ApplicationCommandLineArgs args);

        void Close();

        // std::vector<std::function<void(const std::shared_ptr<fts::Scene>& newScene)>> mPostAttachSceneFunctions;

        static Application& Get()
        {
            return *s_Instance;
        }

        // inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer.get(); }

        inline fts::EventListener& GetAppListner()
        {
            return mAppListener;
        }

        inline Window& GetWindow()
        {
            return *GetWindowPtr();
        }
        inline Window* GetWindowPtr()
        {
            return m_Window;
        }

        void CreateWindowApp(const WindowCreateInfo& props);

        void HandleEvent(const Event& evt);
        void Run();

        // fts::Scene* GetSceneWorld() { FTS_ASSERT(mScene != nullptr);  return mScene.get(); }

        // void EachLayer(std::function<void(Ref<Layer>)> callback);

    protected:
        virtual void Initialize() {};
        virtual void Shutdown() {};

    protected:
        // fts::EventManager mEventManager;

        WindowCreateInfo mSettingApp;
        std::filesystem::path GetAppDirectory();
        ApplicationSpecification mAppSpecification;

        void OnWindowClose(const Event& e);
        void OnWindowResize(const Event& e);
        void OnWindowMinimized(const Event& evt);
        void OnWindowRestored(const Event& evt);
        void OnKeyPressedEvent(const Event& evt);

        // Ref<fts::Scene> mScene;
        fts::EventListener mAppListener;

    public:
        // static void Attach(const std::shared_ptr<fts::Scene>& scene);
        // static std::shared_ptr<fts::Scene> GetActiveScene();

        template <typename T,
                  typename = std::enable_if<std::is_base_of_v<Layer, T>>, typename... Args>
        static std::shared_ptr<T> PushLayer(Args&&... args)
        {
            auto& application = Application::Get();
            if(application.mApplicationStatus != ApplicationStatus::Uninitialized)
            {
                FTS_CORE_ERROR("Unable to push layer! Application already started!");
                return nullptr;
            }
            auto test = GetLayer<T>();
            if(!test)
            {
                test = std::make_shared<T>(std::forward<Args>(args)...);

                if(!std::dynamic_pointer_cast<Layer>(test))
                {
                    FTS_CORE_ERROR("Not a layer!");
                    return nullptr;
                }
                auto& application = Application::Get();
                application.m_LayerStack.push_back(std::dynamic_pointer_cast<Layer>(test));
            }
            return test;
        }

        // template <typename T>
        template <typename T, typename = std::enable_if<std::is_base_of_v<Layer, T>>>
        static std::shared_ptr<T> GetLayer()
        {
            auto& application = Application::Get();
            for(auto& i : application.m_LayerStack)
            {
                auto test = std::dynamic_pointer_cast<T>(i);
                if(test)
                    return test;
            }
            return nullptr;
        }

        // template <typename T>
        template <typename T, typename = std::enable_if<std::is_base_of_v<Layer, T>>>
        static void PopLayer()
        {
            auto& application = Application::Get();

            auto it = application.m_LayerStack.begin();
            while(it != application.m_LayerStack.end())
            {
                auto test = std::dynamic_pointer_cast<T>((*it));
                if(test)
                {
                    (*it)->OnDetach();
                    it = application.m_LayerStack.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        template <typename T, typename = std::enable_if<std::is_base_of_v<Layer, T>>>
        static void PopLayer(std::shared_ptr<Layer> inLayer)
        {
            auto& application = Application::Get();
            auto iter         = std::find(application.m_LayerStack.begin(), application.m_LayerStack.end(), inLayer);

            if(iter != application.m_LayerStack.end())
            {
                (*iter)->OnDetach();
                application.m_LayerStack.erase(iter);
            }
        }

    private:
        virtual void RenderBegin();
        virtual void Render();
        virtual void RenderEnd();

    private:
        // std::shared_ptr<fts::job::ThreadPool>   mThreadPool;
        ApplicationCommandLineArgs m_CommandLineArgs;

        // fts::IRendererDevice* mRendererDevice;

        std::vector<std::shared_ptr<Layer>> m_LayerStack;
        // std::shared_ptr<fts::Scene> mActiveScene;

        bool m_Running   = true;
        bool m_Minimized = false;

        float m_DeltaTime     = 0.0f;
        float m_LastFrameTime = 0.0f;

        // Ref<ImGuiLayer> m_ImGuiLayer;
        Window* m_Window;

        ApplicationStatus mApplicationStatus = ApplicationStatus::Uninitialized;
        GameStatus mGameStatus               = GameStatus::Stop;

    private:
        static Application* s_Instance;
        Ini mSettings;

        void InitSettings();
        void InitEvents();

        //// Create a FileWatcher instance that will check the current folder for changes every 5 seconds
        // sgl::FileWatcher mFileWatcher;
    };

//    // To be defined in CLIENT
// #define RegisterApplication(x) fts::Application* fts::Application::CreateApplicaton()\
       //{\
       //	return new x();\
       //}

//// To be defined in CLIENT
#define RegisterApplication(x)                                                                 \
    fts::Application* fts::Application::CreateApplicaton(fts::ApplicationCommandLineArgs args) \
    {                                                                                          \
        return new x(args);                                                                    \
    }

    // To be defined in CLIENT
     // #define RegisterApplication(x) fts::Application* fts::Application::CreateApplication(const fts::mem::HeapArea& heap,const fts::mem::HeapArea& heapTemp, fts::ApplicationCommandLineArgs args)\
    {\
            return new x(heap, heapTemp, args);\
    }

} // namespace fts

#endif
