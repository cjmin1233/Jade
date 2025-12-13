#pragma once
#include <Jade.h>
#include <Jade/Core/Layer.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Jade/Renderer/Camera/EditorCamera.h"

#include <glm/glm.hpp>

namespace Jade
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

    private:
        void RenderViewport();

        bool OnKeyPressed(KeyPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();

    private:
        Ref<Texture2D> m_Texture;

        Ref<FrameBuffer> m_FrameBuffer;
        glm::vec2 m_ViewportSize;
        glm::vec2 m_ViewportBounds[2];      // min, max bounds of the viewport

        Ref<Scene> m_ActiveScene;

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused, m_ViewportHovered;

        int m_GizmoType;

        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
} // namespace Jade
