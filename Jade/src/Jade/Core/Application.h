#pragma once
#include "Core.h"
#include "Jade/Core/LayerStack.h"
#include "Jade/Core/Window.h"
#include "Jade/Events/Event.h"
#include "Jade/Events/ApplicationEvent.h"

class Layer;

namespace Jade
{
    class JADE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }
        static inline Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running;
        LayerStack m_LayerStack;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}