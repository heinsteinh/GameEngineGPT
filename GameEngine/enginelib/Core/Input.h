#ifndef INPUT_H
#define INPUT_H

#include <Core/KeyCodes.h>
#include <Core/MouseCodes.h>
#include <glm/vec2.hpp>

namespace fts
{

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
} // namespace fts

#endif // INPUT_H
