#pragma once
#include <Jade.h>
#include <Jade/Core/Layer.h>
#include <Jade/Events/KeyEvent.h>
#include <Jade/Events/MouseEvent.h>
#include <Jade/Renderer/Camera/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"

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
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();

    private:
        Ref<Texture2D> m_Texture = nullptr;

        Ref<FrameBuffer> m_FrameBuffer = nullptr;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2] = {};     // min, max bounds of the viewport
        Entity m_HoveredEntity = {};

        Ref<Scene> m_ActiveScene = nullptr;

        EditorCamera m_EditorCamera;

        bool m_ViewportFocused = false, m_ViewportHovered = false;

        int m_GizmoType = -1;

        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
} // namespace Jade
