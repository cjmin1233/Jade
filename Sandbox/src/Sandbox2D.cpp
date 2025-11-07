#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
    , m_ShaderLibrary()
    , m_TriangleVA(nullptr)
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

    m_ShaderLibrary.Load("assets/shaders/VertexPosColor.glsl");
}

void Sandbox2D::OnAttach()
{
    JADE_INFO("Sandbox2D Attached");
}

void Sandbox2D::OnDetach()
{
    JADE_INFO("Sandbox2D Detached");
}

void Sandbox2D::OnUpdate(Jade::Timestep ts)
{               
    // Log the timestep
    JADE_TRACE("Timestep: {0} seconds", ts.GetSeconds());

    // Clear Screen
    Jade::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Jade::RenderCommand::Clear();

    // Update Camera
    m_CameraController.OnUpdate(ts);

    Jade::Renderer::BeginScene(m_CameraController.GetCamera());

    auto vertexPosColorShader = m_ShaderLibrary.Get("VertexPosColor");

    Jade::Renderer::Submit(vertexPosColorShader, m_TriangleVA);

    Jade::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Jade::Event& event)
{
    m_CameraController.OnEvent(event);
}