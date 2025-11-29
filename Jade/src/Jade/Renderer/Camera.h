#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Jade
{
    class Camera
    {
    public:
        Camera();
        Camera(const glm::mat4& projection);

        virtual ~Camera() = default;

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewProjectionMatrix(); }

        const glm::quat& GetRotation() const { return m_Rotation; }
        void SetRotation(const glm::vec3& rotation)
        {
            glm::quat rotX = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
            glm::quat rotY = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
            glm::quat rotZ = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));
            m_Rotation = rotZ * rotY * rotX;
            UpdateViewProjectionMatrix();
        }

    protected:
        void UpdateViewProjectionMatrix();

    protected:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;
        glm::quat m_Rotation;
    };
}
