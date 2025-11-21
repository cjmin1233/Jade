#pragma once
#include "Jade/Core/Layer.h"
#include "Jade/Events/MouseEvent.h"
#include "Jade/Events/KeyEvent.h"
#include "Jade/Events/ApplicationEvent.h"

namespace Jade
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

        void Begin();
        void End();

        inline void SetBlockEvents(bool block) { m_BlockEvents = block; }

    private:
        float m_Time;
        bool m_BlockEvents;
    };
}