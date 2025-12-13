#include "jdpch.h"

#include "Jade/Core/Application.h"
#include "Jade/Core/Log.h"
#include "Jade/Core/Layer.h"
#include "Jade/Core/LayerStack.h"
#include "Jade/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Jade
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Window(Window::Create(WindowProps(specification.Name, specification.Width, specification.Height)))
        , m_ImGuiLayer(CreateRef<ImGuiLayer>())
        , m_LayerStack()
        , m_LastFrameTime(0.0f)
        , m_Running(true)
        , m_Minimized(false)
    {
        JADE_PROFILE_FUNCTION();

        JADE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window->SetEventCallback(JADE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        JADE_PROFILE_FUNCTION();

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            (*it)->OnDetach();
        }

        Renderer::Shutdown();
    }

    void Application::Run()
    {
        JADE_PROFILE_FUNCTION();

        while (m_Running)
        {
            JADE_PROFILE_SCOPE("App RunLoop");

            float time = (float)glfwGetTime(); // Platform::GetTime()
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                {
                    JADE_PROFILE_SCOPE("LayerStack OnUpdate");

                    // Update layers
                    for (auto& layer : m_LayerStack)
                    {
                        layer->OnUpdate(timestep);
                    }
                }

                {
                    JADE_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    // ImGui Rendering
                    m_ImGuiLayer->Begin();
                    for (auto& layer : m_LayerStack)
                    {
                        layer->OnImGuiRender();
                    }
                    m_ImGuiLayer->End();
                }
            }


            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        JADE_PROFILE_FUNCTION();

        // Dispatch events to appropriate handlers
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(JADE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(JADE_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;

            (*it)->OnEvent(e);
        }
    }

    void Application::PushLayer(const Ref<Layer>& layer)
    {
        JADE_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(const Ref<Layer>& layer)
    {
        JADE_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        // Indicate that the event was handled.
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        JADE_PROFILE_FUNCTION();

        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
} // namespace Jade
