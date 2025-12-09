#include "jdpch.h"

#include "Jade/Scene/SceneCamera.h"

namespace Jade
{
    SceneCamera::SceneCamera()
        : Camera()
    {
        UpdateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;

        UpdateProjection();
    }
}
