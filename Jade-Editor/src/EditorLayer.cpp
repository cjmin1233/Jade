#include "EditorLayer.h"

#include <Jade/Scene/SceneSerializer.h>
#include <Jade/Utils/PlatformUtils.h>
#include <Jade/Utils/Utils.h>
#include <Jade/Math/Math.h>

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>

namespace Jade
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
        , m_Texture(nullptr)
        , m_FrameBuffer(nullptr)
        , m_ViewportSize(0.0f, 0.0f)
        , m_ViewportBounds{}
        , m_ActiveScene(nullptr)
        , m_EditorCamera()
        , m_ViewportFocused(false)
        , m_ViewportHovered(false)
        , m_GizmoType(-1)
        , m_SceneHierarchyPanel()
    {
    }

    void EditorLayer::OnAttach()
    {
        JADE_PROFILE_FUNCTION();

        JADE_INFO("EditorLayer Attached");

        m_Texture = Texture2D::Create("assets/textures/test.png");

        // Create Framebuffer
        FrameBufferSpecification fbSpec;
        fbSpec.Width = Application::Get().GetWindow().GetWidth();
        fbSpec.Height = Application::Get().GetWindow().GetHeight();
        fbSpec.AttachmentSpec =
        {
            FrameBufferTextureFormat::RGBA8,
            FrameBufferTextureFormat::RED_INTEGER,
            FrameBufferTextureFormat::Depth,
        };
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        // Setup Editor Camera
        m_EditorCamera = EditorCamera(45.0f, 0.1f, 1000.0f, (float)fbSpec.Width / fbSpec.Height);

        // Create a empty scene
        m_ActiveScene = CreateRef<Scene>();
        // Set the scene in the scene hierarchy panel
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        // Create test entities
        auto textured_square = m_ActiveScene->CreateEntity("Textured Square");
        textured_square.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = m_Texture;
        textured_square.GetComponent<TransformComponent>().Translation = glm::vec3(0.0f, 0.0f, 0.0f);
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
            JADE_PROFILE_SCOPE("Camera Update");

            if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
                // Only resize if both width and height are greater than zero
                (m_ViewportSize.x > 0 && m_ViewportSize.y > 0)
                && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
            {
                // Resize framebuffer
                m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

                // Resize editor camera
                m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

                // Notify scene of viewport resize
                m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
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

        // Update Editor Camera
        m_EditorCamera.OnUpdate(ts);

        // Update scene
        //m_ActiveScene->OnUpdateRuntime(ts);
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        ImVec2 mousePos = ImGui::GetMousePos();
        mousePos.x -= m_ViewportBounds[0].x;
        mousePos.y -= m_ViewportBounds[0].y;

        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        mousePos.y = viewportSize.y - mousePos.y;

        int mouseX = (int)mousePos.x, mouseY = (int)mousePos.y;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_FrameBuffer->ReadPixel(0, mouseX, mouseY);
            int redPixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);

            JADE_CORE_WARN("Pixel Data = {0}", pixelData);
            JADE_CORE_WARN("Red Pixel Data = {0}", redPixelData);
        }

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

#pragma region Stats
        ImGui::Begin("Stats");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        ImGui::Separator();

        const glm::vec3& camPosition = m_EditorCamera.GetPosition();
        const glm::vec3& camFocalPoint = m_EditorCamera.GetFocalPoint();
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camPosition.x, camPosition.y, camPosition.z);
        ImGui::Text("Camera Focal Point: (%.2f, %.2f, %.2f)", camFocalPoint.x, camFocalPoint.y, camFocalPoint.z);

        ImGui::End();
#pragma endregion

        RenderViewport();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);

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

        // Get the renderer ID of the framebuffer's color attachment
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID(0);
        // Get the current cursor position (top-left corner of the viewport)
        ImVec2 viewportOffset = ImGui::GetCursorPos();

        // Draw the image (flipped vertically)
        ImGui::Image((void*)(uintptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

        // Get the window position in screen coordinates
        // (0, 0) -> top-left corner of the screen
        ImVec2 minBound = ImGui::GetWindowPos();

        // Calculate the bounds of the viewport in screen coordinates
        minBound.x += viewportOffset.x;
        minBound.y += viewportOffset.y;
        ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };

        m_ViewportBounds[0] = { minBound.x, minBound.y };
        m_ViewportBounds[1] = { maxBound.x, maxBound.y };

#pragma region Guizmo
        // Guizmo
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        //Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
        // Only draw and manipulate the guizmo if an entity is selected and a camera exists
        if (selectedEntity && m_GizmoType != -1)
        {
            //SceneCamera& camera = cameraEntity.GetComponent<CameraComponent>().Cam;
            //bool isOrthographic = camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic;

            // Setup ImGuizmo
            ImGuizmo::SetOrthographic(false);      // Set orthographic or perspective mode based on camera
            ImGuizmo::SetDrawlist();                        // Draw on top of the current ImGui window

            float windowWidth = ImGui::GetWindowWidth();
            float windowHeight = ImGui::GetWindowHeight();
            // Set the ImGuizmo rectangle to match the viewport
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Runtime camera from entity
            //const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
            //// Invert the camera transform to get the view matrix
            //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            // Editor camera
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            // Get the transform of the selected entity
            TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = transformComponent.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5 for translation/scale
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0f; // Snap to 45 degrees for rotation
            }
            float snapValues[3] = { snapValue, snapValue, snapValue };

            // Manipulate the selected entity's transform
            ImGuizmo::Manipulate(
                glm::value_ptr(cameraView),
                glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType,
                ImGuizmo::LOCAL,
                glm::value_ptr(transform),
                nullptr,
                snap ? snapValues : nullptr
            );

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 outTranslation, outRotation, outScale;
                Math::DecomposeTransform(transform, outTranslation, outRotation, outScale);

                transformComponent.Translation = outTranslation;
                // For continuous rotation, calculate the delta and add it to the existing rotation
                glm::vec3 deltaRotation = outRotation - transformComponent.Rotation;
                transformComponent.Rotation += deltaRotation;
                transformComponent.Scale = outScale;
            }
        }
#pragma endregion

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

        // Gizmo key shortcuts
        case Key::Q:
            if (ImGuizmo::IsUsing())
                break;
            m_GizmoType = -1;
            break;
        case Key::W:
            if (ImGuizmo::IsUsing())
                break;
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Key::E:
            if (ImGuizmo::IsUsing())
                break;
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        case Key::R:
            if (ImGuizmo::IsUsing())
                break;
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
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
