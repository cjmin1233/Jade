#pragma once
#include <Jade/Core/Base.h>
#include <Jade/Scene/Scene.h>
#include <Jade/Scene/Entity.h>

namespace Jade
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void SetLastSelectedEntity(Entity entity) { if (entity) m_LastSelectedEntity = entity; }
        Entity GetSelectedEntity() const { return m_CurrentSelectedEntity; }

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);
        void DrawEntityPopupMenu(Entity entity);
        void DrawComponents(Entity entity);

        void DrawScenePopupMenu();

        void ProcessDeletedEntities();

    private:
        Ref<Scene> m_Context;
        Entity m_CurrentSelectedEntity;
        Entity m_LastSelectedEntity;

        std::vector<Entity> m_DeletedEntities;
    };
}
