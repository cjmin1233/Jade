#include "jdpch.h"

#include "Jade/Scene/SceneCamera.h"

namespace Jade
{
    SceneCamera::SceneCamera()
        : Camera()
        , m_ProjectionType(ProjectionType::Orthographic)
        , m_PerspectiveFOV(45.0f)
        , m_PerspectiveNear(0.01f)
        , m_PerspectiveFar(1000.0f)
        , m_OrthographicSize(10.0f)
        , m_OrthographicNear(0.01f)
        , m_OrthographicFar(1000.0f)
        , m_AspectRatio(16.0f / 9.0f)
    {
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;

        m_PerspectiveFOV = verticalFOV;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;

        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;

        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;

        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection()
    {
        switch (m_ProjectionType)
        {
        case Jade::SceneCamera::ProjectionType::Perspective:
            m_ProjectionMatrix = glm::perspective(
                m_PerspectiveFOV,
                m_AspectRatio,
                m_PerspectiveNear,
                m_PerspectiveFar);
            break;
        case Jade::SceneCamera::ProjectionType::Orthographic:
            m_ProjectionMatrix = glm::ortho(
                -m_OrthographicSize * m_AspectRatio * 0.5f,
                m_OrthographicSize * m_AspectRatio * 0.5f,
                -m_OrthographicSize * 0.5f,
                m_OrthographicSize * 0.5f,
                m_OrthographicNear,
                m_OrthographicFar);
            break;
        default:
            break;
        }

        UpdateViewProjectionMatrix();
    }
}
