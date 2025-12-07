#pragma once
#include "Jade/Renderer/Camera/OrthographicCamera.h"

#include "Jade/Core/Timestep.h"

#include "Jade/Events/MouseEvent.h"
#include "Jade/Events/ApplicationEvent.h"

namespace Jade
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        void OnResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

        void UpdateCameraProjection();

    private:
        float m_AspectRatio;
        float m_ZoomLevel;

        OrthographicCamera m_Camera;
        glm::vec3 m_CameraPosition;
        glm::vec3 m_CameraRotation;

        float m_CameraTranslationSpeed;
        float m_CameraRotationSpeed;

        bool m_Rotation;
        bool m_FitToHeight;
    };
}
