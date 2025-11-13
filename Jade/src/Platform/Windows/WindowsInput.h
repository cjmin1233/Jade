#pragma once
#include "Jade/Core/Input.h"

namespace Jade
{
    class WindowsInput : public Input
    {
    public:
        WindowsInput() = default;
        ~WindowsInput() = default;

    protected:
        virtual bool IsKeyPressedImpl(Key keyCode) override;

        virtual bool IsMouseButtonPressedImpl(Mouse button) override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };
}