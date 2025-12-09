#include "jdpch.h"

#include "Jade/Renderer/Camera/Camera.h"

namespace Jade
{
    Camera::Camera()
    {
        UpdateProjection();
    }

    Camera::Camera(ProjectionType type,
        float fovOrSize,
        float nearClip,
        float farClip,
        float aspectRatio)
        : m_ProjectionType(type)
        , m_AspectRatio(aspectRatio)
    {
        switch (m_ProjectionType)
        {
        case ProjectionType::Perspective:
            m_PerspectiveFOV = fovOrSize;
            m_PerspectiveNear = nearClip;
            m_PerspectiveFar = farClip;
            break;
        case ProjectionType::Orthographic:
            m_OrthographicSize = fovOrSize;
            m_OrthographicNear = nearClip;
            m_OrthographicFar = farClip;
            break;
        default:
            break;
        }

        UpdateProjection();
    }

    void Camera::UpdateProjection()
    {
        JADE_PROFILE_FUNCTION();

        switch (m_ProjectionType)
        {
        case ProjectionType::Perspective:
            m_ProjectionMatrix = glm::perspective(
                glm::radians(m_PerspectiveFOV),
                m_AspectRatio,
                m_PerspectiveNear,
                m_PerspectiveFar);
            break;
        case ProjectionType::Orthographic:
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

        UpdateViewProjection();
    }

    void Camera::UpdateViewProjection()
    {
        JADE_PROFILE_FUNCTION();

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
