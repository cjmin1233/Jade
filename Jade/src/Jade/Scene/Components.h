#pragma once
#include <glm/glm.hpp>

namespace Jade
{
    struct TransformComponent
    {
        // 4x4 transformation matrix
        glm::mat4 Transform = glm::mat4(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}

        // Implicit conversion to glm::mat4
        operator glm::mat4& () { return Transform; }
        operator const glm::mat4& () const { return Transform; }
    };

    struct SpriteRendererComponent
    {
        // RGBA color
        glm::vec4 Color{ 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };
}