#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
    , m_CameraController(1280.0f / 720.0f, true)
    , m_Texture(nullptr)
    , m_SquareSize(1.0f, 1.0f)
    , m_TilingFactor(1.0f)
    , m_SquareColor(1.0f)
{
}

void Sandbox2D::OnAttach()
{
    JADE_INFO("Sandbox2D Attached");

    m_Texture = Jade::Texture2D::Create("assets/textures/test.png");
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
    Jade::Renderer2D::DrawQuad({ 0.5f, 0.0f, 0.0f }, { 0.5f, 1.2f }, { 0.1f, 0.2f, 0.8f, 1.0f });
    Jade::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, m_SquareSize, m_Texture, m_TilingFactor, m_SquareColor);
    Jade::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::DragFloat2("Square Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
    ImGui::DragFloat2("Tiling Factor", glm::value_ptr(m_TilingFactor), 0.1f, 0.1f, 10.0f);
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Jade::Event& event)
{
    m_CameraController.OnEvent(event);
}