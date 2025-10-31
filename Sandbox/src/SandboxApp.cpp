#include <Jade.h>

#include "imgui/imgui.h"

class ExampleLayer : public Jade::Layer
{
public:
    ExampleLayer()
        : Layer("ExampleLayer")
        , m_Shader(nullptr)
        , m_VertexArray(nullptr)
        , m_BlueShader(nullptr)
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
        std::shared_ptr<Jade::VertexBuffer> positionBuffer;
        positionBuffer.reset(Jade::VertexBuffer::Create(positions, sizeof(positions)));
        positionBuffer->SetLayout({
            {Jade::ShaderDataType::Float3, "a_Position"}
            });
        m_VertexArray->AddVertexBuffer(positionBuffer);
        // 색상 VBO
        std::shared_ptr<Jade::VertexBuffer> colorBuffer;
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

        std::shared_ptr<Jade::IndexBuffer> indexBuffer;
        indexBuffer.reset(Jade::IndexBuffer::Create(indices,
            sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);
#pragma endregion

#pragma region Square Setup
        m_SquareVA.reset(Jade::VertexArray::Create());

        float squareVertices[3 * 4] =
        {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };

        std::shared_ptr<Jade::VertexBuffer> squareVB;
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
        std::shared_ptr<Jade::IndexBuffer> squareIB;
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

            out vec4 v_Color;

            void main()
            {
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_Shader.reset(new Jade::Shader(vertexSrc, fragmentSrc));
#pragma endregion

#pragma region Square Shader
        std::string blueShaderVertexSrc = R"(
            #version 330 core
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position; 

            void main()
            {
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_BlueShader.reset(new Jade::Shader(blueShaderVertexSrc,
            blueShaderFragmentSrc));
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

        // Render Triangle
        Jade::Renderer::Submit(m_BlueShader, m_SquareVA);
        Jade::Renderer::Submit(m_Shader, m_VertexArray);
        Jade::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {

    }

    void OnEvent(Jade::Event& event) override
    {

    }

private:
    std::shared_ptr<Jade::Shader> m_Shader;
    std::shared_ptr<Jade::VertexArray> m_VertexArray;

    std::shared_ptr<Jade::Shader> m_BlueShader;
    std::shared_ptr<Jade::VertexArray> m_SquareVA;
    
    Jade::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    glm::vec3 m_CameraRotation;

    float m_CameraMoveSpeed;
    float m_CameraRotationSpeed;
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