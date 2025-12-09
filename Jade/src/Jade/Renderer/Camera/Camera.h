#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Jade
{
    class Camera
    {
    public:
        enum class ProjectionType
        {
            Perspective = 0,
            Orthographic = 1
        };

    public:
        Camera();
        Camera(ProjectionType type,
            float fovOrSize,
            float nearClip,
            float farClip,
            float aspectRatio);

        virtual ~Camera() = default;

        inline void SetPerspective(float fov, float nearClip, float farClip)
        {
            m_ProjectionType = ProjectionType::Perspective;

            m_PerspectiveFOV = fov;
            m_PerspectiveNear = nearClip;
            m_PerspectiveFar = farClip;

            UpdateProjection();
        }
        inline void SetOrthographic(float size, float nearClip, float farClip)
        {
            m_ProjectionType = ProjectionType::Orthographic;

            m_OrthographicSize = size;
            m_OrthographicNear = nearClip;
            m_OrthographicFar = farClip;

            UpdateProjection();
        }

#pragma region Getters/Setters
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }

        float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

        float GetOrthographicSize() const { return m_OrthographicSize; }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }

        void SetProjectionType(ProjectionType type)
        {
            m_ProjectionType = type;
            UpdateProjection();
        }

        void SetPerspectiveFOV(float fov)
        {
            m_PerspectiveFOV = fov;
            UpdateProjection();
        }
        void SetPerspectiveNearClip(float nearClip)
        {
            m_PerspectiveNear = nearClip;
            UpdateProjection();
        }
        void SetPerspectiveFarClip(float farClip)
        {
            m_PerspectiveFar = farClip;
            UpdateProjection();
        }

        void SetOrthographicSize(float size)
        {
            m_OrthographicSize = size;
            UpdateProjection();
        }
        void SetOrthographicNearClip(float nearClip)
        {
            m_OrthographicNear = nearClip;
            UpdateProjection();
        }
        void SetOrthographicFarClip(float farClip)
        {
            m_OrthographicFar = farClip;
            UpdateProjection();
        }
#pragma endregion

    protected:
        void UpdateProjection();
        void UpdateViewProjection();

    protected:
        ProjectionType m_ProjectionType = ProjectionType::Perspective;

        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

        float m_PerspectiveFOV = 45.0f;
        float m_PerspectiveNear = 0.1f;
        float m_PerspectiveFar = 1000.0f;

        float m_OrthographicSize = 1.0f;
        float m_OrthographicNear = -1.0f;
        float m_OrthographicFar = 1000.0f;

        float m_AspectRatio = 16.0f / 9.0f;
    };
}
