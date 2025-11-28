#include "jdpch.h"

#include "SceneHierarchyPanel.h"
#include "Jade/Scene/Components.h"

#include <imgui/imgui.h>

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
            DrawEntityNode(Entity{ entity, m_Context.get()});
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
}
