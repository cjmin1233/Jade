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

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);
        void DrawEntityPopupMenu(Entity entity);
        void DrawComponents(Entity entity);

        void DrawScenePopupMenu();

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
