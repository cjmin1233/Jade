#pragma once
#include <glm/glm.hpp>

namespace Jade::Math
{
    // Decomposes a transformation matrix into translation, rotation (in degrees), and scale components.
    bool DecomposeTransform(const glm::mat4& transform
        , glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}
