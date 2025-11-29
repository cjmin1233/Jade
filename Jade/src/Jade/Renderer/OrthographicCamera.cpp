#include "jdpch.h"

#include "Jade/Renderer/OrthographicCamera.h"

namespace Jade
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : Camera()
    {
        JADE_PROFILE_FUNCTION();

        SetProjection(left, right, bottom, top);
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        JADE_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, 0.0f, 10.0f);

        UpdateViewProjectionMatrix();
    }
}
