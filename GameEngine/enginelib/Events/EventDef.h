#ifndef COREEVENTDEF_H
#define COREEVENTDEF_H

#include <Core/KeyCodes.h>
#include <array>
#include <stdint.h>

namespace fts
{
    enum class Direction
    {
        Left,
        Right,
        Up,
        Down
    };

    struct WindowResizeEvent
    {
        float Width;
        float Height;
    };

    struct WindowCloseEvent
    {
    };
    struct WindowRestoredEvent
    {
    };
    struct WindowMoveEvent
    {
    };
    struct WindowMinimizedEvent
    {
    };
    struct WindowMaximizedEvent
    {
    };
    struct WindowFocusGainedEvent
    {
    };
    struct WindowFocusLostEvent
    {
    };

    struct TextInputEvent
    {
        bool Repeated;
        std::array<char, 32> Text = {0};
    };

    struct MouseButton
    {
        enum class Code : uint8_t
        {
            None = 0,
            Left,
            Right,
            Middle,

            Count,
            Invalid
        };

        enum class State : uint8_t
        {
            Release = 0,
            Press
        };

        enum class WheelDirection : uint8_t
        {
            Normal,
            Flipped
        };
    };

    struct MouseMove
    {
        uint32_t PosX = 0;
        uint32_t PosY = 0;

        float DeltaX = 0.0f;
        float DeltaY = 0.0f;
    };

    struct MouseButtonReleased
    {
        uint32_t X = 0;
        ;
        uint32_t Y = 0;
        ;
        uint8_t Clicks = 0;
        ;

        MouseButton::Code Button       = MouseButton::Code::None;
        MouseButton::State ButtonState = MouseButton::State::Release;
    };

    struct MouseButtonPressed
    {
        uint32_t X = 0;
        uint32_t Y = 0;
        uint8_t Clicks;

        MouseButton::Code Button       = MouseButton::Code::None;
        MouseButton::State ButtonState = MouseButton::State::Release;
    };

    struct MouseWheel
    {
        float Y      = 0.0f;
        float X      = 0.0f;
        float DeltaY = 0.0f;
        float DeltaX = 0.0f;

        MouseButton::WheelDirection direction = MouseButton::WheelDirection::Normal;
    };

    struct KeyEvent
    {
        enum class Type : uint8_t
        {
            Down = 0,
            Up,
            Repeat,

            Count,
            Invalid
        };

        bool KeyCtrl  = false; // Keyboard modifier pressed: Control
        bool KeyShift = false; // Keyboard modifier pressed: Shift
        bool KeyAlt   = false; // Keyboard modifier pressed: Alt

        KeyEvent() = default;
        KeyEvent(KeyEvent::Type type, int32_t key)
            : type(type)
            , key(key)
        {
        }

        KeyEvent::Type type = KeyEvent::Type::Invalid;
        KeyCode key         = 0;
        uint8_t modifiers   = 0;
    };

    struct KeyPressedEvent
    {
        KeyCode Key;
        bool Repeat;
    };

    struct KeyReleasedEvent
    {
        KeyCode Key;
    };

    /// Information about a screen touch event
    class TouchEvent
    {
    public:
        static const unsigned int MaxPointers = 10;
        struct Pointer
        {
            int id;
            float x, y;
            float pressure;
        };

        TouchEvent()
            : count(0)
            , actionIndex(-1)
        {
        }

        unsigned int count;
        int actionIndex;
        Pointer pointers[MaxPointers];

        inline int findPointerIndex(int pointerId) const
        {
            int pointerIndex = -1;
            for(unsigned int i = 0; i < count && i < MaxPointers; i++)
            {
                if(pointers[i].id == pointerId)
                {
                    pointerIndex = i;
                    break;
                }
            }
            return pointerIndex;
        }
    };

} // namespace fts

#endif
