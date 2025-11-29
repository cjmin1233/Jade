#include "jdpch.h"

#include "Jade/Renderer/PerspectiveCamera.h"

namespace Jade
{
    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : Camera()
    {
        SetPerspective(fov, aspectRatio, nearClip, farClip);
    }

    void PerspectiveCamera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

        UpdateViewProjectionMatrix();
    }
}
