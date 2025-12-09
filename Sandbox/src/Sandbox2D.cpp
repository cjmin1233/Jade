#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D")
    , m_Texture(nullptr)
    , m_SquareSize(1.0f, 1.0f)
    , m_TilingFactor(1.0f)
    , m_SquareColor(1.0f)
{
}

void Sandbox2D::OnAttach()
{
    JADE_PROFILE_FUNCTION();

    JADE_INFO("Sandbox2D Attached");

    m_Texture = Jade::Texture2D::Create("assets/textures/test.png");

    Jade::FrameBufferSpecification fbSpec;
    fbSpec.Width = 1280; fbSpec.Height = 720;
}

void Sandbox2D::OnDetach()
{
    JADE_PROFILE_FUNCTION();

    JADE_INFO("Sandbox2D Detached");
}

void Sandbox2D::OnUpdate(Jade::Timestep ts)
{       
    JADE_PROFILE_FUNCTION();

    //{
    //    // Update Camera
    //    JADE_PROFILE_SCOPE("CameraController::OnUpdate");
    //    m_CameraController.OnUpdate(ts);
    //}

    {
        // Render
        JADE_PROFILE_SCOPE("Renderer Prep");

        Jade::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Jade::RenderCommand::Clear();
    }

    {
        // Draw Calls
        JADE_PROFILE_SCOPE("Renderer Draw");
        //Jade::Renderer2D::BeginScene(m_CameraController.GetCamera());

        static float rotation = 0.0f;
        rotation += ts * 50.0f;
        for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 100; ++j)
            {
                Jade::Renderer2D::DrawRotatedQuad({ (float)i - 5.0f, (float)j - 5.0f, 0.0f }, rotation,
                    m_SquareSize, m_Texture, m_TilingFactor, m_SquareColor);
            }
        }

        Jade::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    JADE_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::DragFloat2("Square Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
    ImGui::DragFloat2("Tiling Factor", glm::value_ptr(m_TilingFactor), 0.1f, 0.1f, 10.0f);
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    auto stats = Jade::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
    ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

    ImGui::End();

}

void Sandbox2D::OnEvent(Jade::Event& event)
{
    //m_CameraController.OnEvent(event);
}
