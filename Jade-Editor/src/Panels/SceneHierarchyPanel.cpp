#include "jdpch.h"

#include "SceneHierarchyPanel.h"
#include "Jade/Scene/Components.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Jade
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : m_Context(scene)
        , m_SelectionContext()
    {
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        for (auto entity : m_Context->m_Registry.view<entt::entity>())
        {
            DrawEntityNode(Entity{ entity, m_Context.get() });
        }

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
        {
            m_SelectionContext = {};
        }

        ImGui::End();


        ImGui::Begin("Inspector");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();

        ImGui::ShowDemoWindow();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        // Set up tree node flags
        // If the entity is selected, add the selected flag
        // Always allow opening on arrow click
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
            | ImGuiTreeNodeFlags_OpenOnArrow;

        // If the entity has no children, make it a leaf node
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        // Handle selection
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        // If the node is opened, we would draw its children here
        if (opened)
        {
            // For demonstration, we will just create a dummy child node
            // TODO: Replace this with actual child entity iteration
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
            bool childOpened = ImGui::TreeNodeEx((void*)9817239, flags, (tag + "'s child").c_str());

            if (childOpened)
            {
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        static bool checked = true;
        if (ImGui::Checkbox("##Checkbox", &checked))
        {
            // TODO: active/inactive entity
        }

        if (entity.HasComponent<TagComponent>())
        {
            ImGui::SameLine();
            TagComponent& tag = entity.GetComponent<TagComponent>();
            ImGui::Text("Name:");
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.Tag.c_str());

            ImGui::SameLine();
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag.Tag = std::string(buffer);
            }

            ImGui::Separator();
        }

        if (entity.HasComponent<TransformComponent>())
        {
            auto& transform = entity.GetComponent<TransformComponent>();

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("Translation", glm::value_ptr(transform.Translation), 0.1f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);
            }

            ImGui::Separator();
        }

        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                SceneCamera& camera = entity.GetComponent<CameraComponent>().Cam;

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

                // Projection type combo box
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

                        // Selectable item for each projection type
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            camera.SetProjectionType((SceneCamera::ProjectionType)i);
                        }

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                // Display projection-specific settings
                switch (camera.GetProjectionType())
                {
                case SceneCamera::ProjectionType::Perspective:
                {
                    float fov = camera.GetPerspectiveFOV();

                    if (ImGui::DragFloat("FOV", &fov))
                    {
                        camera.SetPerspectiveFOV(fov);
                    }

                    float nearClip = camera.GetPerspectiveNearClip();

                    if (ImGui::DragFloat("Near Clip", &nearClip))
                    {
                        camera.SetPerspectiveNearClip(nearClip);
                    }

                    float farClip = camera.GetPerspectiveFarClip();

                    if (ImGui::DragFloat("Far Clip", &farClip))
                    {
                        camera.SetPerspectiveFarClip(farClip);
                    }
                }
                break;
                case SceneCamera::ProjectionType::Orthographic:
                {
                    float orthographicSize = camera.GetOrthographicSize();

                    if (ImGui::DragFloat("Size", &orthographicSize))
                    {
                        camera.SetOrthographicSize(orthographicSize);
                    }

                    float nearClip = camera.GetOrthographicNearClip();

                    if (ImGui::DragFloat("Near Clip", &nearClip))
                    {
                        camera.SetOrthographicNearClip(nearClip);
                    }

                    float farClip = camera.GetOrthographicFarClip();

                    if (ImGui::DragFloat("Far Clip", &farClip))
                    {
                        camera.SetOrthographicFarClip(farClip);
                    }
                }
                break;
                }

                ImGui::Separator();
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            auto& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

            if (ImGui::CollapsingHeader("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.Color));
            }
            ImGui::Separator();
        }
    }
}
