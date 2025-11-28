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
        , m_ViewportSize(0.0f, 0.0f)
        , m_ActiveScene(nullptr)
        , m_SquareEntity()
        , m_CameraEntity()
        , m_SecondCameraEntity()
        , m_ViewportFocused(false)
        , m_ViewportHovered(false)
        , m_SceneHierarchyPanel()
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

        m_ActiveScene = CreateRef<Scene>();

        m_SquareEntity = m_ActiveScene->CreateEntity("Blue Square");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(
            glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)
        );
        m_SquareEntity.GetComponent<TransformComponent>().Translation = glm::vec3(-1.0f, 0.0f, 0.0f);

        // Camera Entity
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();

        // Second Camera Entity
        m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip-Space Camera Entity");
        auto& cameraComp = m_SecondCameraEntity.AddComponent<CameraComponent>();
        cameraComp.Primary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            virtual void OnCreate() override
            {
                TransformComponent& transform = GetComponent<TransformComponent>();

                transform.Translation = glm::vec3(rand() % 10 - 5.0f, 0.0f, 0.0f);
            }

            virtual void OnDestroy() override {}

            virtual void OnUpdate(Timestep ts) override
            {
                TransformComponent& transform = GetComponent<TransformComponent>();

                float speed = 5.0f;

                // Camera movement
                if (Input::IsKeyPressed(Key::A))
                {
                    transform.Translation.x -= speed * ts;
                }
                if (Input::IsKeyPressed(Key::D))
                {
                    transform.Translation.x += speed * ts;
                }
                if (Input::IsKeyPressed(Key::E))
                {
                    transform.Translation.y += speed * ts;
                }
                if (Input::IsKeyPressed(Key::Q))
                {
                    transform.Translation.y -= speed * ts;
                }
                if (Input::IsKeyPressed(Key::W))
                {
                    transform.Translation.z += speed * ts;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    transform.Translation.z -= speed * ts;
                }

                // Camera rotation
                if (Input::IsKeyPressed(Key::Z))
                {
                    transform.Rotation.z += 90.0f * ts;
                }
                if (Input::IsKeyPressed(Key::C))
                {
                    transform.Rotation.z -= 90.0f * ts;
                }
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_SecondCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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

            if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
                // Only resize if both width and height are greater than zero
                (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
                && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
            {
                // Resize framebuffer
                m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

                // Resize camera
                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

                // Notify scene of viewport resize
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            }

            // Only update the camera if the viewport is focused
            if (m_ViewportFocused)
            {
                m_CameraController.OnUpdate(ts);
            }
        }

        {
            // Render
            JADE_PROFILE_SCOPE("Renderer Prep");

            Renderer2D::ResetStats();

            m_FrameBuffer->Bind();

            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RenderCommand::Clear();
        }

        // Update scene
        m_ActiveScene->OnUpdate(ts);

        m_FrameBuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        JADE_PROFILE_FUNCTION();

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
            if (ImGui::BeginMenu("File"))
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

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Settings");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        if (m_SquareEntity)
        {
            ImGui::Separator();

            TagComponent& tag = m_SquareEntity.GetComponent<TagComponent>();
            ImGui::Text("Tag: %s", tag.Tag.c_str());

            SpriteRendererComponent& sr = m_SquareEntity.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Square Color", glm::value_ptr(sr.Color));
        }

        ImGui::Separator();
        ImGui::DragFloat3("Camera Transform",
            glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Translation), 0.1f);

        if (ImGui::Checkbox("Use main camera", &m_PrimaryCamera))
        {
            m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        }

        {
            // Camera settings
            auto& cameraComp = m_CameraEntity.GetComponent<CameraComponent>();

            // Drag to change orthographic size
            float orthoSize = cameraComp.Cam.GetOrthographicSize();
            if (ImGui::DragFloat("Camera Ortho Size", &orthoSize))
            {
                cameraComp.Cam.SetOrthographicSize(orthoSize);
            }
        }

        ImGui::End();

        RenderViewport();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

    void EditorLayer::RenderViewport()
    {
        JADE_PROFILE_FUNCTION();

        // remove padding
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        ImGui::PopStyleVar();

        // Check if the viewport is focused / hovered
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        // Enable or disable event blocking based on hover state
        // Viewport hovered -> don't block events
        Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportHovered);

        // Get the size of the viewport panel
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        // Draw the framebuffer's color attachment as an image
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)(uintptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }
} // namespace Jade
