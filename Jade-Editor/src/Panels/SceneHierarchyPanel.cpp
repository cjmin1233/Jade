#include "jdpch.h"

#include "SceneHierarchyPanel.h"

#include <Jade/Scene/Components.h>
#include <Jade/ImGui/ImGuiUtils.h>

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
//#include <imgui/imgui_internal.h>

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
        JADE_PROFILE_FUNCTION();

#pragma region Scene Hierarchy
        ImGui::Begin("Scene Hierarchy");

        if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                m_Context->CreateEntity("Empty Entity");
            }

            ImGui::EndPopup();
        }

        for (auto entity : m_Context->m_Registry.view<entt::entity>())
        {
            DrawEntityNode(Entity{ entity, m_Context.get() });
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        {
            m_SelectionContext = {};
        }


        ImGui::End();
#pragma endregion

#pragma region Inspector
        ImGui::Begin("Inspector");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
#pragma endregion

        ImGui::ShowDemoWindow();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        JADE_PROFILE_FUNCTION();

        auto& tag = entity.GetComponent<TagComponent>().Tag;

        // Set up tree node flags
        // If the entity is selected, add the selected flag
        // Always allow opening on arrow click
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
            | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        // If the entity has no children, make it a leaf node
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        // Handle selection
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        // Mark for deletion
        bool entityDeleted = false;
        // Context menu for the entity
        if (ImGui::BeginPopupContextItem(0, ImGuiPopupFlags_MouseButtonRight))
        {
            if (ImGui::MenuItem("Add Child Entity"))
            {
                // TODO: Implement adding a child entity
            }

            if (ImGui::MenuItem("Delete Entity"))
            {
                entityDeleted = true;
            }

            ImGui::EndPopup();
        }

        // If the node is opened, we would draw its children here
        if (opened)
        {
            // For demonstration, we will just create a dummy child node
            // TODO: Replace this with actual child entity iteration
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;
            bool childOpened = ImGui::TreeNodeEx((void*)9817239, flags, (tag + "'s child").c_str());

            if (childOpened)
            {
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        // If the entity is marked for deletion, delete it
        if (entityDeleted)
        {
            // If the deleted entity is the selected one, clear the selection
            if (m_SelectionContext == entity)
            {
                m_SelectionContext = {};
            }

            m_Context->DestroyEntity(entity);
        }
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity,
        UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;

            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight);

            if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                {
                    removeComponent = true;
                }

                ImGui::EndPopup();
            }

            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
            {
                entity.RemoveComponent<T>();
            }
            
            ImGui::Separator();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        JADE_PROFILE_FUNCTION();

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

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
            {
                ImGuiUtils::DrawVec3Control("Translation", component.Translation);
                ImGuiUtils::DrawVec3Control("Rotation", component.Rotation);
                ImGuiUtils::DrawVec3Control("Scale", component.Scale, 1.0f);
            });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
            {
                SceneCamera& camera = component.Cam;

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
            });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
            });

        // Add Component Button
        ImGui::Spacing();

        ImGuiStyle& style = ImGui::GetStyle();
        const char* label = "Add Component";
        float buttonWidth = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
        float regionWidth = ImGui::GetContentRegionAvail().x;
        float cursorX = ImGui::GetCursorPosX();
        float offsetX = (regionWidth > buttonWidth) ? (regionWidth - buttonWidth) * 0.5f : 0.0f;
        ImGui::SetCursorPosX(cursorX + offsetX);

        if (ImGui::Button(label))
        {
            ImGui::OpenPopup("AddComponent");
        }

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                m_SelectionContext.TryAddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer"))
            {
                m_SelectionContext.TryAddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
