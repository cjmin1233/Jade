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
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
        
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
        bool OnMouseMovedEvent(MouseMovedEvent& event);
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);

        bool OnKeyPressedEvent(KeyPressedEvent& event);
        bool OnKeyReleasedEvent(KeyReleasedEvent& event);
        bool OnKeyTypedEvent(KeyTypedEvent& event);

        bool OnWindowResizeEvent(WindowResizeEvent& event);

    private:
        float m_Time;
    };
}