#pragma once
#include "Jade/Core/Layer.h"
#include "Jade/Events/MouseEvent.h"
#include "Jade/Events/KeyEvent.h"
#include "Jade/Events/ApplicationEvent.h"

namespace Jade
{
    class JADE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time;
    };
}