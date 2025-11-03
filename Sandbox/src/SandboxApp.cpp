#include <Jade.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Jade::Layer
{
public:
    ExampleLayer()
        : Layer("ExampleLayer")
        , m_Shader(nullptr)
        , m_VertexArray(nullptr)
        , m_flatColorShader(nullptr)
        , m_SquareVA(nullptr)
        , m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
        , m_CameraPosition(0.0f)
        , m_CameraRotation(0.0f)
        , m_CameraMoveSpeed(2.0f)
        , m_CameraRotationSpeed(90.0f)
    {   
#pragma region Triangle Setup
        // Vertex Array
        m_VertexArray.reset(Jade::VertexArray::Create());

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
        m_VertexArray->AddVertexBuffer(positionBuffer);
        // 색상 VBO
        Jade::Ref<Jade::VertexBuffer> colorBuffer;
        colorBuffer.reset(Jade::VertexBuffer::Create(colors, sizeof(colors)));
        colorBuffer->SetLayout({
            {Jade::ShaderDataType::Float4, "a_Color"}
            });
        m_VertexArray->AddVertexBuffer(colorBuffer);

        // Index Buffer
        uint32_t indices[] =
        {
            0, 1, 2,
        };

        Jade::Ref<Jade::IndexBuffer> indexBuffer;
        indexBuffer.reset(Jade::IndexBuffer::Create(indices,
            sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region Square Setup
        m_SquareVA.reset(Jade::VertexArray::Create());

        float squareVertices[3 * 4] =
        {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        Jade::Ref<Jade::VertexBuffer> squareVB;
        squareVB.reset(Jade::VertexBuffer::Create(squareVertices,
            sizeof(squareVertices)));
        squareVB->SetLayout({
            {Jade::ShaderDataType::Float3, "a_Position"}
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

        m_Shader.reset(Jade::Shader::Create(vertexSrc, fragmentSrc));
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

        m_flatColorShader.reset(Jade::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragSrc));
#pragma endregion

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
        // Input Handling
        // WASD for xz movement
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::A))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::D))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::W))
            m_CameraPosition.z -= m_CameraMoveSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::S))
            m_CameraPosition.z += m_CameraMoveSpeed * ts;
        // Q/E for y movement
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::Q))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::E))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;

        // Z/X for z-axis rotation
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::Z))
            m_CameraRotation.z += m_CameraRotationSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::X))
            m_CameraRotation.z -= m_CameraRotationSpeed * ts;

        // Clear Screen
        Jade::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Jade::RenderCommand::Clear();

        // Update Camera
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Jade::Renderer::BeginScene(m_Camera);

        std::static_pointer_cast<Jade::OpenGLShader>(m_flatColorShader)->Bind();
        std::static_pointer_cast<Jade::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        // Render Triangle
        for(int y = 0; y < 10; y++)
        {
            for(int x = 0; x < 10; x++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(m_SquareScale));
                Jade::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
            }
        }
        Jade::Renderer::Submit(m_Shader, m_VertexArray);
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

    }

private:
    Jade::Ref<Jade::Shader> m_Shader;
    Jade::Ref<Jade::VertexArray> m_VertexArray;

    Jade::Ref<Jade::Shader> m_flatColorShader;
    Jade::Ref<Jade::VertexArray> m_SquareVA;
    
    Jade::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraRotation;

    float m_CameraMoveSpeed;
    float m_CameraRotationSpeed;

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