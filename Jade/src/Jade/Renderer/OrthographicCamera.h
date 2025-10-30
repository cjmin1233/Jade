#pragma once
#include "Jade/Renderer/Camera.h"

namespace Jade
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void SetProjection(float left, float right, float bottom, float top);
    };
}