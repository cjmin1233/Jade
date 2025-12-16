#include "jdpch.h"

#include "Jade/Renderer/Camera/EditorCamera.h"
#include "Jade/Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Jade
{
    EditorCamera::EditorCamera()
    {
        UpdateView();
    }

    EditorCamera::EditorCamera(float fov, float nearClip, float farClip, float aspectRatio)
        : Camera(ProjectionType::Perspective, fov, nearClip, farClip, aspectRatio)
    {
        UpdateView();
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        JADE_PROFILE_FUNCTION();

        const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;

        // LeftAlt + Left/Right/Middle mouse buttons
        if (Input::IsKeyPressed(Key::LeftAlt))
        {
            if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
                MousePan(delta);
            else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
                MouseRotate(delta);
            else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
                MouseZoom(delta.y);
        }
        // Right mouse button + WASDQE
        else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
        {
            // Rotate camera by rotating around focal point
            MouseRotate(delta);
            m_FocalPoint = m_Position + GetForwardDirection() * m_Distance;

            if (Input::IsKeyPressed(Key::A))
            {
                // Move left
                m_FocalPoint -= GetRightDirection() * (ts * 10.0f);
            }
            if (Input::IsKeyPressed(Key::D))
            {
                // Move right
                m_FocalPoint += GetRightDirection() * (ts * 10.0f);
            }
            if (Input::IsKeyPressed(Key::E))
            {
                // Move up
                m_FocalPoint += GetUpDirection() * (ts * 10.0f);
            }
            if (Input::IsKeyPressed(Key::Q))
            {
                // Move down
                m_FocalPoint -= GetUpDirection() * (ts * 10.0f);
            }
            if (Input::IsKeyPressed(Key::W))
            {
                // Move forward
                m_FocalPoint += GetForwardDirection() * (ts * 10.0f);
            }
            if (Input::IsKeyPressed(Key::S))
            {
                // Move backward
                m_FocalPoint -= GetForwardDirection() * (ts * 10.0f);
            }
        }

        UpdateView();
    }

    void EditorCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        // Dispatch mouse scroll events
        dispatcher.Dispatch<MouseScrolledEvent>(JADE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        // +Y is up
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        // +X is right
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        // -Z is forward
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        // Note: Negate pitch and yaw to match the camera's coordinate system
        // (Looking down -Z by default)
        // -yaw because a positive yaw should turn to the right
        // -pitch because a positive pitch should look up
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

    void EditorCamera::UpdateView()
    {
        m_Position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);

        UpdateViewProjection();
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        float delta = e.GetYOffset() * 0.1f;
        MouseZoom(delta);

        UpdateView();

        // Not handled
        return false;
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();

        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

        // Update yaw and pitch based on mouse movement
        m_Yaw += yawSign * delta.x * RotationSpeed();
        m_Pitch += delta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(float delta)
    {
        // Adjust the distance based on scroll input and zoom speed
        // When distance is small, zoom speed should be small
        m_Distance -= delta * ZoomSpeed();

        // Prevent the camera from getting too close
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        // FocalPoint <------ Distance ------ Camera Position
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        // Calculate pan speed based on viewport dimensions
        // x and y are in range [0, 2.4]
        // Speed factors are calculated using a quadratic function
        // to provide a smooth speed curve
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
        // 0.0366*x^2 - 0.1778*x + 0.3021
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
        // 0.0366*y^2 - 0.1778*y + 0.3021
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;

        // When distance is small, zoom speed should be small
        float speed = distance * distance;

        // Max zoom speed cap
        speed = std::min(speed, 100.0f);

        return speed;
    }
}
