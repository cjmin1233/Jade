#include <Jade.h>

// ---Entry Point-----------------
#include <Jade/Core/EntryPoint.h>
// -------------------------------

#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

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
        m_TriangleVA = Jade::VertexArray::Create();

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
        positionBuffer = Jade::VertexBuffer::Create(positions, sizeof(positions));
        positionBuffer->SetLayout({
            {Jade::ShaderDataType::Float3, "a_Position"}
            });
        m_TriangleVA->AddVertexBuffer(positionBuffer);
        // 색상 VBO
        Jade::Ref<Jade::VertexBuffer> colorBuffer;
        colorBuffer = Jade::VertexBuffer::Create(colors, sizeof(colors));
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
        indexBuffer = Jade::IndexBuffer::Create(indices,
            sizeof(indices) / sizeof(uint32_t));
        m_TriangleVA->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region Square Setup
        m_SquareVA = Jade::VertexArray::Create();

        float squareVertices[5 * 4] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Jade::Ref<Jade::VertexBuffer> squareVB;
        squareVB = Jade::VertexBuffer::Create(squareVertices,
            sizeof(squareVertices));
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
        squareIB = Jade::IndexBuffer::Create(squareIndices,
            sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);
#pragma endregion
        
        m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
        m_ShaderLibrary.Load("assets/shaders/VertexPosColor.glsl");
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
        std::static_pointer_cast<Jade::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


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
        ImGui::ColorEdit4("Square Color 4", glm::value_ptr(m_SquareColor));
        // ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
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

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
    float m_SquareScale = 0.1f;
};


class SandboxApp : public Jade::Application
{
public:
    SandboxApp() 
    {
        PushLayer(new ExampleLayer());
        // PushLayer(new Sandbox2D());
    }
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}