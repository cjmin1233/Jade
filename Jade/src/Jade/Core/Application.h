#pragma once
#include "Core.h"
#include "Jade/Events/Event.h"
#include "Window.h"

namespace Jade
{
    class JADE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}