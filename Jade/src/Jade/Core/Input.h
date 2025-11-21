#pragma once
#include "Jade/Core/Base.h"
#include "Jade/Core/KeyCodes.h"
#include "Jade/Core/MouseCodes.h"

#include <utility>

namespace Jade
{
    class Input
    {
    public:
        static bool IsKeyPressed(Key keyCode);
        static bool IsMouseButtonPressed(Mouse button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}