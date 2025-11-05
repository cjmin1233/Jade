#include <Jade.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Jade::Layer
{
public:
    ExampleLayer()
        : Layer("ExampleLayer")
        , m_ShaderLibrary()
        , m_TriangleVA(nullptr)
        , m_SquareVA(nullptr)
        , m_Texture(nullptr)
        , m_CameraController(1280.0f / 720.0f, true)
    {   
#pragma region Triangle Setup
        // Vertex Array
        m_TriangleVA.reset(Jade::VertexArray::Create());

        // 위치 데이터 (3 vertices, 3 floats each)
        float positions[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };

        // 색상 데이터 (3 vertices, 4 floats each)
        float colors[3 * 4] = {
            0.8f, 0.2f, 0.8f, 1.0f,
            0.2f, 0.3f, 0.8f, 1.0f,
            0.8f, 0.8f, 0.2f, 1.0f,
        };

        // 위치 VBO
        Jade::Ref<Jade::VertexBuffer> positionBuffer;
        positionBuffer.reset(Jade::VertexBuffer::Create(positions, sizeof(positions)));
        positionBuffer->SetLayout({
            {Jade::ShaderDataType::Float3, "a_Position"}
            });
        m_TriangleVA->AddVertexBuffer(positionBuffer);
        // 색상 VBO
        Jade::Ref<Jade::VertexBuffer> colorBuffer;
        colorBuffer.reset(Jade::VertexBuffer::Create(colors, sizeof(colors)));
        colorBuffer->SetLayout({
            {Jade::ShaderDataType::Float4, "a_Color"}
            });
        m_TriangleVA->AddVertexBuffer(colorBuffer);

        // Index Buffer
        uint32_t indices[] =
        {
            0, 1, 2,
        };

        Jade::Ref<Jade::IndexBuffer> indexBuffer;
        indexBuffer.reset(Jade::IndexBuffer::Create(indices,
            sizeof(indices) / sizeof(uint32_t)));
        m_TriangleVA->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region Square Setup
        m_SquareVA.reset(Jade::VertexArray::Create());

        float squareVertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Jade::Ref<Jade::VertexBuffer> squareVB;
        squareVB.reset(Jade::VertexBuffer::Create(squareVertices,
            sizeof(squareVertices)));
        squareVB->SetLayout({
            {Jade::ShaderDataType::Float3, "a_Position"},
            {Jade::ShaderDataType::Float2, "a_TexCoord"}
            });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] =
        {
            0, 1, 2,
            2, 3, 0
        };
        Jade::Ref<Jade::IndexBuffer> squareIB;
        squareIB.reset(Jade::IndexBuffer::Create(squareIndices,
            sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);
#pragma endregion

#pragma region Triangle Shader
        std::string vertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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
#pragma endregion

#pragma region Square Shader
        std::string flatColorShaderVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position; 

            void main()
            {
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
                v_Position = a_Position;
            }
        )";

        std::string flatColorShaderFragSrc = R"(
            #version 330 core
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0);
            }
        )";
#pragma endregion

        m_ShaderLibrary.Add(Jade::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc));
        m_ShaderLibrary.Add(Jade::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragSrc));

        auto textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");
        m_Texture = Jade::Texture2D::Create("assets/textures/test.png");

        std::static_pointer_cast<Jade::OpenGLShader>(textureShader)->Bind();
        std::static_pointer_cast<Jade::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnAttach() override
    {
        JADE_INFO("ExampleLayer Attached");
    }

    void OnDetach() override
    {
        JADE_INFO("ExampleLayer Detached");
    }

    void OnUpdate(Jade::Timestep ts) override
    {
        // Log the timestep
        JADE_TRACE("Timestep: {0} seconds", ts.GetSeconds());

        // Clear Screen
        Jade::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Jade::RenderCommand::Clear();

        // Update Camera
        m_CameraController.OnUpdate(ts);

        Jade::Renderer::BeginScene(m_CameraController.GetCamera());

        auto textureShader = m_ShaderLibrary.Get("texture");
        auto flatColorShader = m_ShaderLibrary.Get("FlatColor");    
        auto vertexPosColorShader = m_ShaderLibrary.Get("VertexPosColor");

        std::static_pointer_cast<Jade::OpenGLShader>(flatColorShader)->Bind();
        std::static_pointer_cast<Jade::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);


        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 10; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(m_SquareScale));
                Jade::Renderer::Submit(flatColorShader, m_SquareVA, transform);
            }
        }

        m_Texture->Bind(0);
        Jade::Renderer::Submit(textureShader, m_SquareVA,
            glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Jade::Renderer::Submit(vertexPosColorShader, m_TriangleVA);
        Jade::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::DragFloat("Square Scale", &m_SquareScale, 0.01f, 0.01f, 0.1f, "%.2f");
        ImGui::End();
    }

    void OnEvent(Jade::Event& event) override
    {
        m_CameraController.OnEvent(event);
    }

private:
    Jade::ShaderLibrary m_ShaderLibrary;

    Jade::Ref<Jade::VertexArray> m_TriangleVA;
    Jade::Ref<Jade::VertexArray> m_SquareVA;

    Jade::Ref<Jade::Texture2D> m_Texture;
    
    Jade::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
    float m_SquareScale = 0.1f;
};

class SandboxApp : public Jade::Application
{
public:
    SandboxApp() 
    {
        PushLayer(new ExampleLayer());
    }
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}