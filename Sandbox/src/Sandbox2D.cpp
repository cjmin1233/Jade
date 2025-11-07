#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
    , m_CameraController(1280.0f / 720.0f, true)
{
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

    // Update Camera
    m_CameraController.OnUpdate(ts);

    // Render
    Jade::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Jade::RenderCommand::Clear();

    Jade::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Jade::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Jade::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Jade::Event& event)
{
    m_CameraController.OnEvent(event);
}