#pragma once
#include "Jade/Scene/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Jade
{
    struct TagComponent
    {
        // Entity tag/name
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };

    struct TransformComponent
    {
        // Position, rotation and scale
        glm::vec3 Translation = glm::vec3(0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            : Translation(translation) {}

        // Get the transformation matrix (S * R * T)
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Translation)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
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

    struct CameraComponent
    {
        // The scene camera
        SceneCamera Cam;
        // Is this the primary camera?
        bool Primary = true;
        // Should the aspect ratio be fixed?
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}