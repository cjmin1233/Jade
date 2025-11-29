#pragma once
#include "Jade/Core/KeyCodes.h"
#include "Jade/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace Jade
{
    class Input
    {
    public:
        static bool IsKeyPressed(Key keyCode);
        static bool IsMouseButtonPressed(Mouse button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
