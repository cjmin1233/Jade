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
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        inline static bool IsKeyPressed(Key keyCode)
        {
            return s_Instance->IsKeyPressedImpl(keyCode);
        }
        inline static bool IsMouseButtonPressed(Mouse button)
        {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }
        inline static std::pair<float, float> GetMousePosition()
        {
            return s_Instance->GetMousePositionImpl();
        }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

        static Scope<Input> Create();

    protected:
        Input() = default;

        virtual bool IsKeyPressedImpl(Key keyCode) = 0;

        virtual bool IsMouseButtonPressedImpl(Mouse button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Scope<Input> s_Instance;
    };
}