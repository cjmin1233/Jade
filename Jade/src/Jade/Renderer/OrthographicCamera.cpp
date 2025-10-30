#include "jdpch.h"

#include "Jade/Renderer/OrthographicCamera.h"

namespace Jade
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : Camera()
    {
        SetProjection(left, right, bottom, top);
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {                         
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, 0.1f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}