#include "jdpch.h"

#include "Jade/Core/Application.h"
#include "Jade/Core/Log.h"
#include "Jade/Core/Layer.h"
#include "Jade/Core/LayerStack.h"
#include "Jade/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Jade
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Window(std::unique_ptr<Window>(Window::Create()))
        , m_ImGuiLayer(new ImGuiLayer())
        , m_LayerStack()
        , m_VertexArray(0)
        , m_Running(true)
        , m_Shader(nullptr)
        , m_VertexBuffer(nullptr)
        , m_IndexBuffer(nullptr)
    {
        JADE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(JADE_BIND_EVENT_FN(Application::OnEvent));

        PushOverlay(m_ImGuiLayer);

        // Vertex Array
        // glGenVertexArrays(1, &m_VertexArray);
        glCreateVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        // -1.0f to 1.0f
        float vertices[3 * 4]=
        {
            -1.f, -1.f, 0.0f,
             1.f, -1.f, 0.0f,
             1.f,  1.f, 0.0f,
             -1.f,  1.f, 0.0f,
        };

        // Vertex Buffer
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float), nullptr);

        uint32_t indices[] = 
        { 
            0, 1, 2, 
            2, 3, 0
        };

        // Index Buffer
        m_IndexBuffer.reset(IndexBuffer::Create(indices, 
            sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
                v_Position = a_Position;
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(v_Position*0.5+0.5, 1.0);
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(.2f, .2f, .2f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->Bind();

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), 
                GL_UNSIGNED_INT, nullptr);

            for(Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            // ImGui Rendering
            m_ImGuiLayer->Begin();
            for(Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(JADE_BIND_EVENT_FN(Application::OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
} // namespace Jade