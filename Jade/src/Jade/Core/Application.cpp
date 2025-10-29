#include "jdpch.h"

#include "Jade/Core/Application.h"
#include "Jade/Core/Log.h"
#include "Jade/Core/Layer.h"
#include "Jade/Core/LayerStack.h"
#include "Jade/Renderer/Buffer.h"
#include "Jade/Renderer/VertexArray.h"
#include "Jade/Renderer/Renderer.h"

namespace Jade
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Window(std::unique_ptr<Window>(Window::Create()))
        , m_ImGuiLayer(new ImGuiLayer())
        , m_LayerStack()
        , m_Running(true)
        , m_Shader(nullptr)
        , m_VertexArray(nullptr)
        , m_BlueShader(nullptr)
        , m_SquareVA(nullptr)
    {
        JADE_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(JADE_BIND_EVENT_FN(Application::OnEvent));

        PushOverlay(m_ImGuiLayer);

#pragma region Triangle Setup
        // Vertex Array
        m_VertexArray.reset(VertexArray::Create());

        // 위치 데이터 (3 vertices, 3 floats each)
        float positions[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
        };

        // 색상 데이터 (3 vertices, 4 floats each)
        float colors[3 * 4] = {
            0.8f, 0.2f, 0.8f, 1.0f,
            0.2f, 0.3f, 0.8f, 1.0f,
            0.8f, 0.8f, 0.2f, 1.0f,
        };

        // 위치 VBO
        std::shared_ptr<VertexBuffer> positionBuffer;
        positionBuffer.reset(VertexBuffer::Create(positions, sizeof(positions)));
        positionBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
            });
        m_VertexArray->AddVertexBuffer(positionBuffer);
        // 색상 VBO
        std::shared_ptr<VertexBuffer> colorBuffer;
        colorBuffer.reset(VertexBuffer::Create(colors, sizeof(colors)));
        colorBuffer->SetLayout({
            {ShaderDataType::Float4, "a_Color"}
            });
        m_VertexArray->AddVertexBuffer(colorBuffer);

        // Index Buffer
        uint32_t indices[] = 
        { 
            0, 1, 2, 
        };

        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, 
            sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region Square Setup
        m_SquareVA.reset(VertexArray::Create());
        
        float squareVertices[3 * 4] = 
        {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, 
            sizeof(squareVertices)));
        squareVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, 
            sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);
#pragma endregion

#pragma region Triangle Shader
        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec4 v_Color;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
                v_Color = a_Color;
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            in vec4 v_Color;

            void main()
            {
                color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
#pragma endregion

#pragma region Square Shader
        std::string blueShaderVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position; 

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
                v_Position = a_Position;
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.2, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Shader(blueShaderVertexSrc, 
            blueShaderFragmentSrc));
#pragma endregion
    }

    void Application::Run()
    {
        while (m_Running)
        {
            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
            RenderCommand::Clear();

            // Render environment.
            Renderer::BeginScene();

            // Render Objects.
            m_BlueShader->Bind();
            Renderer::Submit(m_SquareVA);

            m_Shader->Bind();
            Renderer::Submit(m_VertexArray);

            Renderer::EndScene();

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