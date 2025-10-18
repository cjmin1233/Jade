#include "jdpch.h"

#include "Application.h"
#include "Jade/Events/ApplicationEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Jade
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }
    Application::~Application()
    {
    }
    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1.f, 0.f, 1.f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
} // namespace Jade