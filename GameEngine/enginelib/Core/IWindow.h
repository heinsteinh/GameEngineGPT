
#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace fts
{

    enum WindowFlags
    {
        NONE       = 1 << 0,
        MAXIMIZED  = 1 << 1,
        FULLSCREEN = 1 << 2
    };

    struct WindowCreateInfo
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        int Flags;
        bool VSync                    = true;
        glm::vec4 clearColor          = {0.0f, 0.0f, 0.0f, 1.0f};
        uint32_t Opengl_Major_Version = 4;
        uint32_t Opengl_Minor_Version = 6;
        WindowCreateInfo(const std::string& title = "Fts Engine",
                         uint32_t width           = 1024, // 1024,
                         uint32_t height          = 768,  // 768, //840,
                         bool vsync               = true,
                         int flags                = WindowFlags::NONE)
            : Title(title)
            , Width(width)
            , Height(height)
            , Flags(flags)
            , VSync(vsync)
        {
        }
    };

    class Event;
    class GraphicsContext;
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(const Event&)>;

        virtual ~Window() = default;

        virtual void ProcessEvents() = 0;
        virtual void SwapBuffers()   = 0;

        /* Access Window parameters */
        virtual uint32_t GetWidth() const                        = 0;
        virtual uint32_t GetHeight() const                       = 0;
        virtual uint32_t GetXpos() const                         = 0;
        virtual uint32_t GetYpos() const                         = 0;
        virtual glm::vec4 GetClearColor() const                  = 0;
        virtual std::pair<int32_t, int32_t> GetWindowPos() const = 0;

        // Window attributes
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const        = 0;

        virtual void SetEventCallback(const EventCallbackFn& CallbackFunc) = 0;

        virtual void* GetNativeWindow() const               = 0;
        virtual GraphicsContext* GetGraphicsContext() const = 0;

        static Window* Create(const WindowCreateInfo& props = WindowCreateInfo());

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;

        virtual std::string GetTitle() const                  = 0;
        virtual void SetTitle(const std::string& title)       = 0;
        virtual void SetIcon(const std::string& iconFilepath) = 0;
        virtual void TakeScreenshot() const                   = 0;
    };
} // namespace fts

namespace entry
{
    struct WindowHandle
    {
        uint16_t idx;
    };
    inline bool isValid(WindowHandle _handle)
    {
        return UINT16_MAX != _handle.idx;
    }

    struct GamepadHandle
    {
        uint16_t idx;
    };
    inline bool isValid(GamepadHandle _handle)
    {
        return UINT16_MAX != _handle.idx;
    }

    struct MouseButton
    {
        enum Enum
        {
            None,
            Left,
            Middle,
            Right,

            Count
        };
    };

    struct GamepadAxis
    {
        enum Enum
        {
            LeftX,
            LeftY,
            LeftZ,
            RightX,
            RightY,
            RightZ,

            Count
        };
    };

    struct Modifier
    {
        enum Enum
        {
            None       = 0,
            LeftAlt    = 0x01,
            RightAlt   = 0x02,
            LeftCtrl   = 0x04,
            RightCtrl  = 0x08,
            LeftShift  = 0x10,
            RightShift = 0x20,
            LeftMeta   = 0x40,
            RightMeta  = 0x80,
        };
    };

    struct Key
    {
        enum Enum
        {
            None = 0,
            Esc,
            Return,
            Tab,
            Space,
            Backspace,
            Up,
            Down,
            Left,
            Right,
            Insert,
            Delete,
            Home,
            End,
            PageUp,
            PageDown,
            Print,
            Plus,
            Minus,
            LeftBracket,
            RightBracket,
            Semicolon,
            Quote,
            Comma,
            Period,
            Slash,
            Backslash,
            Tilde,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            NumPad0,
            NumPad1,
            NumPad2,
            NumPad3,
            NumPad4,
            NumPad5,
            NumPad6,
            NumPad7,
            NumPad8,
            NumPad9,
            Key0,
            Key1,
            Key2,
            Key3,
            Key4,
            Key5,
            Key6,
            Key7,
            Key8,
            Key9,
            KeyA,
            KeyB,
            KeyC,
            KeyD,
            KeyE,
            KeyF,
            KeyG,
            KeyH,
            KeyI,
            KeyJ,
            KeyK,
            KeyL,
            KeyM,
            KeyN,
            KeyO,
            KeyP,
            KeyQ,
            KeyR,
            KeyS,
            KeyT,
            KeyU,
            KeyV,
            KeyW,
            KeyX,
            KeyY,
            KeyZ,

            GamepadA,
            GamepadB,
            GamepadX,
            GamepadY,
            GamepadThumbL,
            GamepadThumbR,
            GamepadShoulderL,
            GamepadShoulderR,
            GamepadUp,
            GamepadDown,
            GamepadLeft,
            GamepadRight,
            GamepadBack,
            GamepadStart,
            GamepadGuide,

            Count
        };
    };

    struct Suspend
    {
        enum Enum
        {
            WillSuspend,
            DidSuspend,
            WillResume,
            DidResume,

            Count
        };
    };

    const char* getName(Key::Enum _key);

    struct MouseState
    {
        MouseState()
            : m_mx(0)
            , m_my(0)
            , m_mz(0)
        {
            for(uint32_t ii = 0; ii < entry::MouseButton::Count; ++ii)
            {
                m_buttons[ii] = entry::MouseButton::None;
            }
        }

        int32_t m_mx;
        int32_t m_my;
        int32_t m_mz;
        uint8_t m_buttons[entry::MouseButton::Count];
    };

    struct GamepadState
    {
        GamepadState()
        {
            memset(m_axis, 0, sizeof(m_axis));
        }

        int32_t m_axis[entry::GamepadAxis::Count];
    };

    struct WindowState
    {
        WindowState()
            : m_width(0)
            , m_height(0)
            , m_nwh(NULL)
        {
            m_handle.idx = UINT16_MAX;
        }

        WindowHandle m_handle;
        uint32_t m_width;
        uint32_t m_height;
        MouseState m_mouse;
        void* m_nwh;
    };
} // namespace entry

#endif
