#include "jdpch.h"

#include "Jade/Renderer/Camera.h"

namespace Jade
{
    Camera::Camera() 
        : m_ProjectionMatrix(1.0f)
        , m_ViewProjectionMatrix(1.0f)
        , m_Position()
        , m_Rotation()
    {
        UpdateViewProjectionMatrix();
    }

    Camera::Camera(const glm::mat4& projection)
        : m_ProjectionMatrix(projection)
        , m_ViewProjectionMatrix(1.0f)
        , m_Position()
        , m_Rotation()
    {
        UpdateViewProjectionMatrix();
    }

    void Camera::UpdateViewProjectionMatrix()
    {
        JADE_PROFILE_FUNCTION();

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 transform = translation * glm::mat4_cast(m_Rotation);
        glm::mat4 view = glm::inverse(transform);

        m_ViewProjectionMatrix = m_ProjectionMatrix * view;
    }
}