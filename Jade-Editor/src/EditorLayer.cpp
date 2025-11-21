#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Jade 
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
        , m_CameraController(Application::Get().GetWindow().GetAspectRatio(), true)
        , m_Texture(nullptr)
        , m_SquareSize(1.0f, 1.0f)
        , m_TilingFactor(1.0f)
        , m_SquareColor(1.0f)
        , m_FrameBuffer(nullptr)
    {
    }

    void EditorLayer::OnAttach()
    {
        JADE_PROFILE_FUNCTION();

        JADE_INFO("EditorLayer Attached");

        m_Texture = Texture2D::Create("assets/textures/test.png");

        FrameBufferSpecification fbSpec;
        fbSpec.Width = Application::Get().GetWindow().GetWidth(); 
        fbSpec.Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    }

    void EditorLayer::OnDetach()
    {
        JADE_PROFILE_FUNCTION();

        JADE_INFO("EditorLayer Detached");
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {       
        JADE_PROFILE_FUNCTION();

        {
            // Update Camera
            JADE_PROFILE_SCOPE("CameraController::OnUpdate");
            m_CameraController.OnUpdate(ts);
        }

        {
            // Render
            JADE_PROFILE_SCOPE("Renderer Prep");

            m_FrameBuffer->Bind();

            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RenderCommand::Clear();
        }

        {
            // Draw Calls
            JADE_PROFILE_SCOPE("Renderer Draw");
            Renderer2D::BeginScene(m_CameraController.GetCamera());

            static float rotation = 0.0f;
            rotation += ts * 50.0f;
            for (int i = 0; i < 100; ++i)
            {
                for (int j = 0; j < 100; ++j)
                {
                    Renderer2D::DrawRotatedQuad({ (float)i - 5.0f, (float)j - 5.0f, 0.0f }, rotation,
                        m_SquareSize, m_Texture, m_TilingFactor, m_SquareColor);
                }
            }

            Renderer2D::EndScene();

            m_FrameBuffer->Unbind();
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        JADE_PROFILE_FUNCTION();

        // Note: Switch this to true to enable docking
        static bool dockingEnabled = true;

        if (dockingEnabled)
        {
            // TL;DR; this demo is more complicated than what most users you would normally use.
            // If we remove all options we are showcasing, this demo would become a simple call to ImGui::DockSpaceOverViewport() !!
            // In this specific demo, we are not using DockSpaceOverViewport() because:

            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            static bool dockspaceOpen = false;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);

                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

                window_flags |=
                    ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoCollapse
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove;
                window_flags |= 
                    ImGuiWindowFlags_NoBringToFrontOnFocus 
                    | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // Submit the DockSpace
            // REMINDER: THIS IS A DEMO FOR ADVANCED USAGE OF DockSpace()!
            // MOST REGULAR APPLICATIONS WILL SIMPLY WANT TO CALL DockSpaceOverViewport(). READ COMMENTS ABOVE.
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            // Show demo options and help
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Files"))
                {
                    if (ImGui::MenuItem("Exit"))
                    {
                        Application::Get().Close();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Options"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                    ImGui::Separator();

                    if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
                    if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
                    if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
                    if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                    if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                    if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::Begin("Settings");
            ImGui::DragFloat2("Square Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
            ImGui::DragFloat2("Tiling Factor", glm::value_ptr(m_TilingFactor), 0.1f, 0.1f, 10.0f);
            ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad Count: %d", stats.QuadCount);
            ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
            ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

            ImGui::Separator();
            ImGui::Text("FrameBuffer Info:");
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            glm::vec2 fbSize = glm::vec2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height);
            ImGui::Image((void*)(uintptr_t)textureID, ImVec2(fbSize.x, fbSize.y), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();

            ImGui::End();
        }

        else
        {
            ImGui::Begin("Settings");
            ImGui::DragFloat2("Square Size", glm::value_ptr(m_SquareSize), 0.1f, 0.1f, 10.0f);
            ImGui::DragFloat2("Tiling Factor", glm::value_ptr(m_TilingFactor), 0.1f, 0.1f, 10.0f);
            ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

            auto stats = Renderer2D::GetStats();
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quad Count: %d", stats.QuadCount);
            ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
            ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
        
            ImGui::Separator();
            ImGui::Text("FrameBuffer Info:");
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            glm::vec2 fbSize = glm::vec2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height);
            ImGui::Image((void*)(uintptr_t)textureID, ImVec2(fbSize.x, fbSize.y), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
        }
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }
} // namespace Jade