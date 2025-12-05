#include "EditorLayer.h"

#include <Jade/Scene/SceneSerializer.h>
#include <Jade/Utils/PlatformUtils.h>
#include <Jade/Utils/Utils.h>

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

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

#if 0
        m_SquareEntity = m_ActiveScene->CreateEntity("Blue Square");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(
            glm::vec4(0.2f, 0.3f, 0.8f, 1.0f)
        );
        m_SquareEntity.GetComponent<TransformComponent>().Translation = glm::vec3(-1.0f, 0.0f, 0.0f);

        auto red_square = m_ActiveScene->CreateEntity("Red Square");
        red_square.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.3f, 0.2f, 1.0f));
        red_square.GetComponent<TransformComponent>().Translation = glm::vec3(1.0f, 0.0f, 0.0f);

        // Camera Entity
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();
        m_CameraEntity.GetComponent<TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 5.0f);

        // Second Camera Entity
        m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera Entity");
        auto& cameraComp = m_SecondCameraEntity.AddComponent<CameraComponent>();
        cameraComp.Primary = false;
        m_SecondCameraEntity.GetComponent<TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 5.0f);

        class CameraController : public ScriptableEntity
        {
        public:
            virtual void OnCreate() override
            {
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
                    transform.Translation.z -= speed * ts;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    transform.Translation.z += speed * ts;
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
#endif
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

                //// Resize camera
                //m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

                // Notify scene of viewport resize
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            }

            //// Only update the camera if the viewport is focused
            //if (m_ViewportFocused)
            //{
            //    m_CameraController.OnUpdate(ts);
            //}
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

#pragma region DockSpace
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

        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;

        style.WindowMinSize.x = 200.0f;

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        // Show demo options and help
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                {
                    OpenScene();
                }

                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                {
                    SaveScene();
                }

                if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
                {
                    SaveSceneAs();
                }

                //if (ImGui::MenuItem("Serialize"))
                //{
                //    SceneSerializer serializer(m_ActiveScene);

                //    serializer.Serialize("assets/scenes/");
                //}

                //if (ImGui::MenuItem("Deserialize"))
                //{
                //    SceneSerializer serializer(m_ActiveScene);

                //    serializer.Deserialize("assets/scenes/" + m_ActiveScene->GetName() + ".jade");
                //}

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

        ImGui::End();
#pragma endregion

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Stats");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        ImGui::Separator();

        if (ImGui::Checkbox("Use main camera", &m_PrimaryCamera))
        {
            m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        }

        ImGui::End();

        RenderViewport();

    }

    void EditorLayer::OnEvent(Event& event)
    {
        //m_CameraController.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(JADE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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
        // If the viewport is focused and hovered, don't block events
        Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        // Get the size of the viewport panel
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        // Draw the framebuffer's color attachment as an image
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)(uintptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Short-circuit repeated key presses
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
        case Key::N:
        {
            if (control)
            {
                NewScene();
            }
        }
        break;
        case Key::O:
        {
            if (control)
            {
                OpenScene();
            }
        }
        break;
        case Key::S:
        {
            if (control)
            {
                if (shift)
                {
                    SaveSceneAs();
                }
                else
                {
                    SaveScene();
                }
            }
        }
        break;
        }

        // Not handled
        return false;
    }

    // Create a new empty scene
    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    // Open a scene from a file with a user-specified path
    void EditorLayer::OpenScene()
    {
        auto filepath = FileDialogs::OpenFile("Jade Scene (*.jade)\0*.jade\0");

        if (filepath.has_value())
        {
            const std::string sceneName = Utils::GetFileName(*filepath);

            m_ActiveScene = CreateRef<Scene>(sceneName);    // Set scene name based on file name
            m_ActiveScene->SetFilePath(*filepath);   // Set scene file path
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);

            serializer.Deserialize(*filepath);
        }
    }

    void EditorLayer::SaveScene()
    {
        if (m_ActiveScene)
        {
            std::string filepath = m_ActiveScene->GetFilePath();

            if (!filepath.empty())
            {
                SceneSerializer serializer(m_ActiveScene);

                serializer.Serialize(filepath);
            }
        }
    }

    // Save the active scene to a file with a user-specified path
    void EditorLayer::SaveSceneAs()
    {
        auto filepath = FileDialogs::SaveFile("Jade Scene (*.jade)\0*.jade\0");

        if (filepath.has_value())
        {
            const std::string sceneName = Utils::GetFileName(*filepath);

            m_ActiveScene->SetName(sceneName); // Set scene name based on file name
            m_ActiveScene->SetFilePath(*filepath);   // Set scene file path
            SceneSerializer serializer(m_ActiveScene);

            serializer.Serialize(*filepath);
        }
    }
} // namespace Jade
