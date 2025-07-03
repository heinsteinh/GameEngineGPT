#pragma once

#include <Core/IWindow.h>
#include <Events/EventDef.h>
#include <SDL.h>
#include <string>

namespace fts::win32
{

    class Window_windows : public Window
    {
    public:
        Window_windows(const WindowCreateInfo& props);
        virtual ~Window_windows();

        void ProcessEvents() override;
        void SwapBuffers() override;

        glm::vec4 GetClearColor() const override
        {
            return mData.clearColor;
        }
        uint32_t GetWidth() const override
        {
            return mData.Width;
        }
        uint32_t GetHeight() const override
        {
            return mData.Height;
        }
        uint32_t GetXpos() const override
        {
            return mData.Xpos;
        }
        uint32_t GetYpos() const override
        {
            return mData.Ypos;
        }
        std::pair<int32_t, int32_t> GetWindowPos() const override;
        MultiSampleLevel GetMSAA() const override;
        ;

        inline void SetEventCallback(const EventCallbackFn& callback) override
        {
            mData.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        void SetTitle(const std::string& title) override;

        inline virtual void* GetNativeWindow() const override
        {
            return (void*)mSdlWindow;
        }
        inline virtual GraphicsContext* GetGraphicsContext() const override
        {
            return mContext.get();
        }

        virtual void BeginFrame() override;
        virtual void EndFrame() override;

        std::string GetTitle() const override;
        // void SetIcon(const std::string& iconFilepath) override;
        // void TakeScreenshot() const override;

    private:
        void Init(const WindowCreateInfo& props);
        void Shutdown();
        void HandleEvents();

        MouseButton::Code ConvertToButton(uint8_t button);

    private:
        SDL_Window* mSdlWindow;
        std::unique_ptr<GraphicsContext> mContext;

        struct WindowData
        {
            std::string Title = "";
            uint32_t Width = 0, Height = 0;
            uint32_t Xpos = 0, Ypos = 0;
            bool VSync           = false;
            int MSAA             = 4;
            glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            EventCallbackFn EventCallback;
        };

        WindowData mData;
    };
} // namespace fts::win32
