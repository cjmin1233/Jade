#pragma once
#include "Jade/Core/Core.h"
#include "Jade/Core/KeyCodes.h"
#include "Jade/Core/MouseCodes.h"

#include <utility>

namespace Jade
{
    class JADE_API Input
    {
    public:
        inline static bool IsKeyPressed(Key::KeyCode keyCode)
        {
            return s_Instance->IsKeyPressedImpl(keyCode);
        }

        inline static bool IsMouseButtonPressed(MouseCode button)
        {
            return s_Instance->IsMouseButtonPressedImpl(button);
        }
        inline static std::pair<float, float> GetMousePosition()
        {
            return s_Instance->GetMousePositionImpl();
        }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(Key::KeyCode keyCode) PURE;

        virtual bool IsMouseButtonPressedImpl(MouseCode button) PURE;
        virtual std::pair<float, float> GetMousePositionImpl() PURE;
        virtual float GetMouseXImpl() PURE;
        virtual float GetMouseYImpl() PURE;

    protected:
        Input() = default;
        virtual ~Input() = default;

    private:
        static Input* s_Instance;
    };
}