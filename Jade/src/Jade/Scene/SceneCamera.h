#pragma once
#include "Jade/Renderer/Camera/Camera.h"

namespace Jade
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType
        {
            Perspective = 0,
            Orthographic = 1
        };

    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void SetOrthographic(float size, float nearClip, float farClip);

        void SetViewportSize(uint32_t width, uint32_t height);

#pragma region Getters/Setters
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
            RecalculateProjection();
        }

        void SetPerspectiveFOV(float fov)
        {
            m_PerspectiveFOV = fov;
            RecalculateProjection();
        }
        void SetPerspectiveNearClip(float nearClip)
        {
            m_PerspectiveNear = nearClip;
            RecalculateProjection();
        }
        void SetPerspectiveFarClip(float farClip)
        {
            m_PerspectiveFar = farClip;
            RecalculateProjection();
        }

        void SetOrthographicSize(float size)
        {
            m_OrthographicSize = size;
            RecalculateProjection();
        }
        void SetOrthographicNearClip(float nearClip)
        {
            m_OrthographicNear = nearClip;
            RecalculateProjection();
        }
        void SetOrthographicFarClip(float farClip)
        {
            m_OrthographicFar = farClip;
            RecalculateProjection();
        }
#pragma endregion

    private:
        void RecalculateProjection();

    private:
        ProjectionType m_ProjectionType;

        float m_PerspectiveFOV;
        float m_PerspectiveNear, m_PerspectiveFar;

        float m_OrthographicSize;
        float m_OrthographicNear, m_OrthographicFar;

        float m_AspectRatio;
    };
}
