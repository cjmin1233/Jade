#pragma once
#include "Jade/Renderer/Camera.h"

namespace Jade
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
        void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
    };
}
