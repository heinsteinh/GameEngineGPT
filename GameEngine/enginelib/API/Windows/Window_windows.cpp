#include <API/Windows/Window_windows.h>
#include <ftspch.h>

// #include "Renderer/GraphicsContext.h"
// #include "Utils/EngineVersion.h"

// #include <stb_image.h>
// #include <stb_image_write.h>

#include <Core/InputCore.h>
#include <Utils/IncludeDef.h>

// #include "EventSystem/Events.h"

#include <SDL2/SDL.h>

namespace entry
{
    static uint8_t translateKeyModifiers(uint16_t _sdl)
    {
        uint8_t modifiers = 0;
        modifiers |= _sdl & KMOD_LALT ? Modifier::LeftAlt : 0;
        modifiers |= _sdl & KMOD_RALT ? Modifier::RightAlt : 0;
        modifiers |= _sdl & KMOD_LCTRL ? Modifier::LeftCtrl : 0;
        modifiers |= _sdl & KMOD_RCTRL ? Modifier::RightCtrl : 0;
        modifiers |= _sdl & KMOD_LSHIFT ? Modifier::LeftShift : 0;
        modifiers |= _sdl & KMOD_RSHIFT ? Modifier::RightShift : 0;
        modifiers |= _sdl & KMOD_LGUI ? Modifier::LeftMeta : 0;
        modifiers |= _sdl & KMOD_RGUI ? Modifier::RightMeta : 0;
        return modifiers;
    }

    static uint8_t translateKeyModifierPress(uint16_t _key)
    {
        uint8_t modifier;
        switch(_key)
        {
        case SDL_SCANCODE_LALT:
        {
            modifier = Modifier::LeftAlt;
        }
        break;
        case SDL_SCANCODE_RALT:
        {
            modifier = Modifier::RightAlt;
        }
        break;
        case SDL_SCANCODE_LCTRL:
        {
            modifier = Modifier::LeftCtrl;
        }
        break;
        case SDL_SCANCODE_RCTRL:
        {
            modifier = Modifier::RightCtrl;
        }
        break;
        case SDL_SCANCODE_LSHIFT:
        {
            modifier = Modifier::LeftShift;
        }
        break;
        case SDL_SCANCODE_RSHIFT:
        {
            modifier = Modifier::RightShift;
        }
        break;
        case SDL_SCANCODE_LGUI:
        {
            modifier = Modifier::LeftMeta;
        }
        break;
        case SDL_SCANCODE_RGUI:
        {
            modifier = Modifier::RightMeta;
        }
        break;
        default:
        {
            modifier = 0;
        }
        break;
        }

        return modifier;
    }

    static uint8_t s_translateKey[256];

    static void initTranslateKey(uint16_t _sdl, Key::Enum _key)
    {
        FTS_ASSERT_MSG(_sdl < sizeof(s_translateKey), "Out of bounds {}.", _sdl);
        s_translateKey[_sdl & 0xff] = (uint8_t)_key;
    }

    static Key::Enum translateKey(SDL_Scancode _sdl)
    {
        return (Key::Enum)s_translateKey[_sdl & 0xff];
    }
} // namespace entry

namespace fts
{
    Window* Window::Create(const WindowCreateInfo& props)
    {
        return new fts::win32::Window_windows(props); // std::make_shared<fts::win32::Window_windows>(props); //
    }
} // namespace fts

// https://github.com/AkliDev/Alternate-Engine/blob/952594d009be9b97558f87ba25c082fc6df36dd7/Alternate/src/Platform/Windows/WindowsWindow.cpp

namespace fts::win32
{
    // static bool s_SDLWindowCount = 0;
    static uint8_t s_SDLWindowCount = 0;

    Window_windows::Window_windows(const WindowCreateInfo& props)
    {
        Init(props);
    }

    Window_windows::~Window_windows()
    {
        Shutdown();
        FTS_CORE_TRACE("Shutdown complete");
    }

    void Window_windows::Init(const WindowCreateInfo& props)
    {

        mData.Title      = props.Title;
        mData.Width      = props.Width;
        mData.Height     = props.Height;
        mData.clearColor = props.clearColor;

        // FTS_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        FTS_CORE_INFO("Platform SDL Window:");
        FTS_CORE_INFO("  Title: {0}", props.Title);
        FTS_CORE_INFO("  Size: ({0}, {1})", props.Width, props.Height);

        if(s_SDLWindowCount == 0)
        {
            FTS_CORE_INFO("SDLInit");

            int success = SDL_Init(SDL_INIT_EVERYTHING);
            FTS_ASSERT_MSG(!success, "Could not initialise SDL!");
        }

        {
            HZ_PROFILE_SCOPE("sdlCreateWindow");

            mSdlWindow = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, props.Width, props.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
            ++s_SDLWindowCount;

            if(!mSdlWindow)
            {
                terminate();
                throw std::exception("Failed to create SDL window.");
            }
        }

        ContextCreateInfo ctxInfo;
        ctxInfo.MSAA = props.MSAA;

        mContext = fts::GraphicsContext::Create(this, &ctxInfo);
        mContext->Init();

        int xx, yy;
        SDL_GetWindowPosition(mSdlWindow, &xx, &yy);
        mData.Xpos = xx;
        mData.Ypos = yy;

        mData.MSAA = static_cast<int>(props.MSAA);

        SDL_SetWindowData(mSdlWindow, "WindowData", &mData);
        SetVSync(mData.VSync);

        VersionInfo version;
        int major = 0, minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::stringstream strMsg;
        strMsg << props.Title << " \nOpenGL " + std::to_string(major) + "." << std::to_string(minor);
        strMsg << " \nBuild version: " + std::to_string(version.getMinorVersion()) + "." + std::to_string(version.getPatch()) + "." + version.getExtra();

        SDL_SetWindowTitle(mSdlWindow, strMsg.str().c_str());

        mContext->MakeCurrentContext();

        // Reset the context for Renderthread example
        SDL_GL_MakeCurrent(mSdlWindow, nullptr);
    }

    void Window_windows::Shutdown()
    {
        mContext->DeleteRenderContext();
        SDL_DestroyWindow(mSdlWindow);

        mContext.reset();
        mContext = nullptr;

        if(s_SDLWindowCount == 0)
        {
            SDL_Quit();
        }
    }

    void Window_windows::ProcessEvents()
    {
        HZ_PROFILE_FUNCTION();

        HandleEvents();
    }

    void Window_windows::SwapBuffers()
    {
        HZ_PROFILE_FUNCTION();

        mContext->SwapBuffers();
    }

    void Window_windows::SetVSync(bool enabled)
    {
        FTS_CORE_INFO("Set Vsync : {0}", enabled);
        if(enabled)
            SDL_GL_SetSwapInterval(1);
        else
            SDL_GL_SetSwapInterval(0);

        mData.VSync = enabled;
    }

    bool Window_windows::IsVSync() const
    {
        return mData.VSync;
    }

    void Window_windows::SetTitle(const std::string& title)
    {
        mData.Title = title;
        SDL_SetWindowTitle(mSdlWindow, mData.Title.c_str());
    }

    std::string Window_windows::GetTitle() const
    {
        return SDL_GetWindowTitle(mSdlWindow);
    }

    std::pair<int32_t, int32_t> Window_windows::GetWindowPos() const
    {
        int32_t x, y;
        SDL_GetWindowPosition(mSdlWindow, &x, &y);
        return {x, y};
    }

    void Window_windows::BeginFrame()
    {
        mContext->BeginFrame();
    }

    void Window_windows::EndFrame()
    {
        HZ_PROFILE_FUNCTION();
        mContext->EndFrame();    // i.e. "submit"
        mContext->SwapBuffers(); // i.e. "present"
    }

    MouseButton::Code Window_windows::ConvertToButton(uint8_t button)
    {
        if(button == SDL_BUTTON_LEFT)
        {
            return MouseButton::Code::Left;
        }
        if(button == SDL_BUTTON_MIDDLE)
        {
            return MouseButton::Code::Middle;
        }
        else if(button == SDL_BUTTON_RIGHT)
        {
            return MouseButton::Code::Right;
        }
        return MouseButton::Code::Count;
    }

    void Window_windows::HandleEvents()
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {

#ifdef FTS_IMGUI_
            ImGui_ImplSDL2_ProcessEvent(&event);
#endif

            switch(event.type)
            {
            case SDL_QUIT:
            {
                fts::EventSystem::PushEvent(FTS_EVENT_QUIT, 0);
                break;
            }
            case SDL_WINDOWEVENT:
            {
                if(event.window.windowID == SDL_GetWindowID(mSdlWindow))
                {

                    switch(event.window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        uint32_t width      = static_cast<uint32_t>(event.window.data1);
                        uint32_t height     = static_cast<uint32_t>(event.window.data2);

                        winData.Width  = width;
                        winData.Height = height;
                        WindowResizeEvent ALT_event(width, height);
                        Event evtData(FTS_EVENT_WINDOW_RESIZE, ALT_event);
                        winData.EventCallback(evtData);

                        int xx, yy;
                        SDL_GetWindowPosition(mSdlWindow, &xx, &yy);
                        mData.Xpos = xx;
                        mData.Ypos = yy;

                        mData.Width  = width;
                        mData.Height = height;
                        WindowResizeEvent DFG_event;
                        DFG_event.Width  = float(event.window.data1);
                        DFG_event.Height = float(event.window.data2);

                        fts::EventSystem::PushEvent(FTS_EVENT_WINDOW_RESIZE, DFG_event);

                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE:
                    {
                        WindowCloseEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowCloseEvent>(FTS_EVENT_WINDOW_CLOSE, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_CLOSE, DFG_event);
                        winData.EventCallback(evtData);
                        break;
                    }
                    case SDL_WINDOWEVENT_MINIMIZED:
                    {
                        WindowMinimizedEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowMinimizedEvent>(FTS_EVENT_WINDOW_MINIMIZED, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_MINIMIZED, DFG_event);
                        winData.EventCallback(evtData);

                        break;
                    }
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    {
                        WindowMaximizedEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowMaximizedEvent>(FTS_EVENT_WINDOW_MAXIMIZED, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_MAXIMIZED, DFG_event);
                        winData.EventCallback(evtData);

                        break;
                    }
                    case SDL_WINDOWEVENT_MOVED:
                    {
                        WindowMoveEvent DFG_event;
                        fts::EventSystem::PushEvent(FTS_EVENT_WINDOW_MOVED, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_MOVED, DFG_event);
                        winData.EventCallback(evtData);
                        break;
                    }
                    case SDL_WINDOWEVENT_RESTORED:
                    {
                        WindowRestoredEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowRestoredEvent>(FTS_EVENT_WINDOW_RESTORE, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_RESTORE, DFG_event);
                        winData.EventCallback(evtData);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        WindowFocusGainedEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowFocusGainedEvent>(FTS_EVENT_WINDOW_FOCUS_GAINED /*"WindowFocusGained"*/, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_FOCUS_GAINED, DFG_event);
                        winData.EventCallback(evtData);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        WindowFocusLostEvent DFG_event;
                        fts::EventSystem::PushEvent<WindowFocusLostEvent>(FTS_EVENT_WINDOW_FOCUS_LOST /*"WindowFocusLost"*/, DFG_event);

                        WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                        Event evtData(FTS_EVENT_WINDOW_FOCUS_LOST, DFG_event);
                        winData.EventCallback(evtData);
                        break;
                    }

                    case SDL_FINGERDOWN:
                    case SDL_FINGERMOTION:
                    case SDL_FINGERUP:
                    {
                        TouchEvent touchEvent_;

                        // https://github.com/nCine/nCine/blob/a0f44001b2e04d775a472b90c4743c83dcb5f2a7/src/input/SdlInputManager.cpp#L37
                        touchEvent_.count = SDL_GetNumTouchFingers(event.tfinger.touchId);
                        for(unsigned int i = 0; i < touchEvent_.count; i++)
                        {
                            SDL_Finger* finger           = SDL_GetTouchFinger(event.tfinger.touchId, i);
                            TouchEvent::Pointer& pointer = touchEvent_.pointers[i];
                            pointer.id                   = static_cast<int>(finger->id);
                            pointer.x                    = this->GetWidth() * finger->x;
                            pointer.y                    = this->GetHeight() * (1.0f - finger->y);
                            pointer.pressure             = finger->pressure;
                        }
                        break;
                    }
                    }
                }
                break;
            }
            case SDL_TEXTINPUT:
            {
                TextInputEvent DFG_event;
                ;
                static_assert(sizeof(event.text.text) <= sizeof(DFG_event.Text));
                std::memcpy(DFG_event.Text.data(), event.text.text, sizeof(event.text.text));

                EventSystem::PushEvent(FTS_EVENT_TEXT_INPUT, DFG_event);

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_TEXT_INPUT, DFG_event);
                winData.EventCallback(evtData);
                break;
            }
            case SDL_KEYDOWN:
            {

                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
                }
                else
                {

                    const SDL_KeyboardEvent& kev = event.key;
                    { // Experiments

                        uint8_t modifiers    = entry::translateKeyModifiers(kev.keysym.mod);
                        entry::Key::Enum key = entry::translateKey(kev.keysym.scancode);

                        /* FTS_CORE_TRACE("SDL scancode {0}, key {1}, name {2}, key name {3}"
                             , kev.keysym.scancode
                             , key
                             , SDL_GetScancodeName(kev.keysym.scancode)
                             , SDL_GetKeyName(kev.keysym.scancode)
                         );*/

                        /// If you only press (e.g.) 'shift' and nothing else, then key == 'shift', modifier == 0.
                        /// Further along, pressing 'shift' + 'ctrl' would be: key == 'shift', modifier == 'ctrl.
                        if(0 == key && 0 == modifiers)
                        {
                            modifiers = entry::translateKeyModifierPress(kev.keysym.scancode);
                        }

                        if(entry::Key::Return == key)
                        {
                            uint8_t pressedChar[4];
                            pressedChar[0] = 0x0d;

                            TextInputEvent PressedChar_event;
                            ;
                            static_assert(sizeof(event.text.text) <= sizeof(PressedChar_event.Text));
                            std::memcpy(PressedChar_event.Text.data(), pressedChar, sizeof(pressedChar));
                            EventSystem::PushEvent(FTS_EVENT_TEXT_INPUT, PressedChar_event);
                        }
                        else if(entry::Key::Backspace == key)
                        {
                            uint8_t pressedChar[4];
                            pressedChar[0] = 0x08;

                            TextInputEvent PressedChar_event;
                            ;
                            static_assert(sizeof(event.text.text) <= sizeof(PressedChar_event.Text));
                            std::memcpy(PressedChar_event.Text.data(), pressedChar, sizeof(pressedChar));
                            EventSystem::PushEvent(FTS_EVENT_TEXT_INPUT, PressedChar_event);
                        }
                    }

                    fts::KeyEvent keyEvent;
                    keyEvent.key = static_cast<KeyCode>(kev.keysym.scancode);
                    if(event.key.repeat != 0)
                    {
                        keyEvent.type = fts::KeyEvent::Type::Repeat;
                    }
                    else
                    {
                        keyEvent.type = fts::KeyEvent::Type::Down;
                    }

                    // https://github.com/nCine/nCine/blob/a0f44001b2e04d775a472b90c4743c83dcb5f2a7/src/input/SdlInputManager.cpp#L37
                    keyEvent.KeyShift  = ((SDL_GetModState() & KMOD_SHIFT) != 0);
                    keyEvent.KeyCtrl   = ((SDL_GetModState() & KMOD_CTRL) != 0);
                    keyEvent.KeyAlt    = ((SDL_GetModState() & KMOD_ALT) != 0);
                    keyEvent.modifiers = entry::translateKeyModifiers(kev.keysym.mod);
                    ;

                    fts::EventSystem::PushEvent<KeyEvent>(FTS_EVENT_KEY, keyEvent);

                    KeyPressedEvent keyPressed;
                    keyPressed.Key    = static_cast<KeyCode>(event.key.keysym.scancode);
                    keyPressed.Repeat = event.key.repeat != 0 ? true : false;
                    fts::EventSystem::PushEvent<KeyPressedEvent>(FTS_EVENT_KEY_PRESSED, keyPressed);
                    // fts::core::EventManager::Get()->QueueEvent(fts::core::IEventDataPtr(new EventData_KeyboardDown(keyPressed.Key, keyPressed.Repeat)));

                    WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                    Event evtData(FTS_EVENT_KEY_PRESSED, keyPressed);
                    winData.EventCallback(evtData);
                }

                break;
            }
            case SDL_KEYUP:
            {
                WindowData& data = *(WindowData*)SDL_GetWindowData(mSdlWindow, "data");

                fts::KeyEvent keyEvent(fts::KeyEvent::Type::Up, event.key.keysym.scancode);

                keyEvent.KeyShift  = ((SDL_GetModState() & KMOD_SHIFT) != 0);
                keyEvent.KeyCtrl   = ((SDL_GetModState() & KMOD_CTRL) != 0);
                keyEvent.KeyAlt    = ((SDL_GetModState() & KMOD_ALT) != 0);
                keyEvent.modifiers = entry::translateKeyModifiers(event.key.keysym.mod);
                ;

                fts::EventSystem::PushEvent<KeyEvent>(FTS_EVENT_KEY, keyEvent);

                KeyReleasedEvent keyReleased;
                keyReleased.Key = static_cast<KeyCode>(event.key.keysym.scancode);
                fts::EventSystem::PushEvent<KeyReleasedEvent>(FTS_EVENT_KEY_RELEASED, keyReleased);

                // fts::core::EventManager::Get()->QueueEvent(fts::core::IEventDataPtr(new EventData_KeyboardUp(keyReleased.Key)));

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_KEY_RELEASED, keyReleased);
                winData.EventCallback(evtData);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                const SDL_MouseButtonEvent& mev = event.button;

                MouseButtonPressed mouseDownEvent;
                mouseDownEvent.X      = mev.x;
                mouseDownEvent.Y      = mev.y;
                mouseDownEvent.Clicks = mev.clicks;
                // mouseDownEvent.Button = ConvertToButton(mev.button);
                mouseDownEvent.ButtonState = mev.state == SDL_PRESSED ? MouseButton::State::Press : MouseButton::State::Release;

                switch(mev.button)
                {
                default:
                case SDL_BUTTON_LEFT:
                    mouseDownEvent.Button = MouseButton::Code::Left;
                    break;
                case SDL_BUTTON_MIDDLE:
                    mouseDownEvent.Button = MouseButton::Code::Middle;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseDownEvent.Button = MouseButton::Code::Right;
                    break;
                }

                fts::EventSystem::PushEvent<MouseButtonPressed>(FTS_EVENT_MOUSE_PRESSED, mouseDownEvent);

                // auto evt = fts::IEventDataPtr(new EventData_MouseDown(mev.x, mev.y, mouseDownEvent.Button));
                // fts::core::EventManager::Get()->QueueEvent(evt);

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_MOUSE_PRESSED, mouseDownEvent);
                winData.EventCallback(evtData);
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                const SDL_MouseButtonEvent& mev = event.button;

                MouseButtonReleased mouseUpEvent;
                mouseUpEvent.X      = mev.x;
                mouseUpEvent.Y      = mev.y;
                mouseUpEvent.Clicks = mev.clicks;
                // mouseUpEvent.Button = ConvertToButton(mev.button);;
                mouseUpEvent.ButtonState = mev.state == SDL_PRESSED ? MouseButton::State::Press : MouseButton::State::Release;

                switch(mev.button)
                {
                default:
                case SDL_BUTTON_LEFT:
                    mouseUpEvent.Button = MouseButton::Code::Left;
                    break;
                case SDL_BUTTON_MIDDLE:
                    mouseUpEvent.Button = MouseButton::Code::Middle;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseUpEvent.Button = MouseButton::Code::Right;
                    break;
                }

                fts::EventSystem::PushEvent(FTS_EVENT_MOUSE_RELEASED, mouseUpEvent);

                // auto evt = fts::core::IEventDataPtr(new EventData_MouseUp(mev.x, mev.y, mouseUpEvent.Button));
                // fts::core::EventManager::Get()->QueueEvent(evt);

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_MOUSE_RELEASED, mouseUpEvent);
                winData.EventCallback(evtData);
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                const SDL_MouseWheelEvent& mev = event.wheel;

                MouseWheel mouseWheelEvent;
                mouseWheelEvent.Y         = mev.y;
                mouseWheelEvent.X         = mev.x;
                mouseWheelEvent.DeltaY    = mev.y;
                mouseWheelEvent.direction = (mev.direction == SDL_MOUSEWHEEL_NORMAL ? fts::MouseButton::WheelDirection::Normal : fts::MouseButton::WheelDirection::Flipped);

                fts::EventSystem::PushEvent<MouseWheel>(FTS_EVENT_MOUSE_SCROLLED, mouseWheelEvent);

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_MOUSE_SCROLLED, mouseWheelEvent);
                winData.EventCallback(evtData);
                break;
            }
            case SDL_MOUSEMOTION:
            {
                const SDL_MouseMotionEvent& mev = event.motion;

                MouseMove mouseEvent;
                mouseEvent.PosX   = mev.x;
                mouseEvent.PosY   = mev.y;
                mouseEvent.DeltaX = mev.xrel;
                mouseEvent.DeltaY = mev.yrel;

                fts::EventSystem::PushEvent<MouseMove>(FTS_EVENT_MOUSE_MOVED, mouseEvent);
                // auto evt = fts::core::IEventDataPtr(new EventData_MouseMove(mev.x, mev.y));
                // fts::core::EventManager::Get()->QueueEvent(evt);

                WindowData& winData = *(WindowData*)SDL_GetWindowData(mSdlWindow, "WindowData");
                Event evtData(FTS_EVENT_MOUSE_MOVED, mouseEvent);
                winData.EventCallback(evtData);
                break;
            }

            case SDL_JOYAXISMOTION:
            {
                // https://github.com/gcavalieri/link/blob/08b5e9c851337aaa25cc03c5ec90bd17c441889c/examples/common/entry/entry_sdl.cpp
                /* const SDL_JoyAxisEvent& jev = event.jaxis;
                 GamepadHandle handle = findGamepad(jev.which);
                 if (isValid(handle))
                 {
                     GamepadAxis::Enum axis = translateGamepadAxis(jev.axis);
                     m_gamepad[handle.idx].update(m_eventQueue, defaultWindow, handle, axis, jev.value);
                 }*/
                break;
            }
            }
        }
    }

    /*
    void Window_windows::SetIcon(const std::string& iconFilepath)
    {
        int width = 0;
        int height = 0;
        std::byte* iconData = reinterpret_cast<std::byte*>(stbi_load(iconFilepath.c_str(), &width, &height, nullptr, STBI_rgb_alpha));

        if (iconData == nullptr)
        {
            std::ostringstream errorMessageStream;
            errorMessageStream << "ERROR: Failed to load icon file.";

            throw std::runtime_error(errorMessageStream.str());
        }

        Uint32 redMask = 0x00'00'00'FF;
        Uint32 blueMask = 0x00'00'FF'00;
        Uint32 greenMask = 0x00'FF'00'00;
        Uint32 alphaMask = 0xFF'00'00'00;

        if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            redMask = 0xFF'00'00'00;
            blueMask = 0x00'FF'00'00;
            greenMask = 0x00'00'FF'00;
            alphaMask = 0x00'00'00'FF;
        }

        SDL_Surface* iconSurface = SDL_CreateRGBSurfaceFrom(iconData, width, height, 32, width * 4, redMask, blueMask, greenMask, alphaMask);
        SDL_SetWindowIcon(mSdlWindow, iconSurface);
        SDL_FreeSurface(iconSurface);

        stbi_image_free(iconData);
    }


    //Only work with multiple of 4 window size
    void Window_windows::TakeScreenshot() const
    {
        FTS_CORE_INFO("Taking screenshot...");

        auto sceenShotDir = getPathRelativeRoot("screenshot");

        if (!std::filesystem::is_directory(sceenShotDir) || !std::filesystem::exists(sceenShotDir)) {
            std::filesystem::create_directory(sceenShotDir);
        }

        uint32_t width = GetWidth();
        uint32_t height = GetHeight();

        FTS_ASSERT_MSG((height % 4 == 0 && width % 4 == 0), "Wrong window size");

        constexpr unsigned int PixelChannelCount = 3u;
        std::vector<std::byte> imageData(width * height * PixelChannelCount);

        glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

        const long long currentTime = std::chrono::system_clock::now().time_since_epoch().count();
        const std::string filename = sceenShotDir + "/screenshot_" + std::to_string(currentTime) + ".png";

        stbi_flip_vertically_on_write(true);
        stbi_write_png(filename.c_str(), width, height, PixelChannelCount, imageData.data(), width * PixelChannelCount * sizeof(unsigned char));



      //  unsigned char* pixels = new unsigned char[3 * width * height];
      //  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      //  stbi_flip_vertically_on_write(true);
      //  stbi_write_png(filename.c_str(), width, height, 3, pixels, 3 * width * sizeof(unsigned char));
      //  delete[] pixels;

        FTS_CORE_INFO("Screenshot taken.");
    }
    */

} // namespace fts::win32
