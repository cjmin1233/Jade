#pragma once
#include "Jade/Renderer/Camera/Camera.h"
#include "Jade/Core/Timestep.h"
#include "Jade/Events/Event.h"
#include "Jade/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Jade
{
    class EditorCamera : public Camera
    {
    public:
        EditorCamera();
        EditorCamera(float fov, float nearClip, float farClip, float aspectRatio);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        inline void SetViewportSize(float width, float height)
        {
            m_ViewportWidth = width;
            m_ViewportHeight = height;
            m_AspectRatio = width / height;

            UpdateProjection();
        }

        glm::vec3 GetUpDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::vec3& GetFocalPoint() const { return m_FocalPoint; }
        glm::quat GetOrientation() const;

        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }

    private:
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& e);

        void MousePan(const glm::vec2& delta);      // Pan the camera based on mouse movement
        void MouseRotate(const glm::vec2& delta);   // Rotate the camera based on mouse movement
        void MouseZoom(float delta);                // Zoom the camera in/out based on scroll input

        glm::vec3 CalculatePosition() const;

        std::pair<float, float> PanSpeed() const;
        inline float RotationSpeed() const { return 0.8f; } // Fixed rotation speed
        float ZoomSpeed() const;

    private:
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f }; // Camera position in world space
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f }; // The point the camera is looking at

        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };   // For tracking mouse movement during interactions

        float m_Distance = 10.0f;           // Distance from the camera to the focal point
        float m_Pitch = 0.0f, m_Yaw = 0.0f; // Euler angles for camera orientation

        float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
    };
}
