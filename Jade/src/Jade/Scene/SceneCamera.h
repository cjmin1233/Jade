#pragma once
#include "Jade/Renderer/Camera/Camera.h"

namespace Jade
{
    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetViewportSize(uint32_t width, uint32_t height);
    };
}
