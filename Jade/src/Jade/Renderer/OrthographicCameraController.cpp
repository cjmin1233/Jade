#include "jdpch.h"

#include "Jade/Renderer/OrthographicCameraController.h"

#include "Jade/Core/Input.h"
#include "Jade/Core/KeyCodes.h"

namespace Jade
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio)
        , m_ZoomLevel(1.0f)
        , m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
        , m_CameraPosition()
        , m_CameraRotation()
        , m_CameraTranslationSpeed(5.0f)
        , m_CameraRotationSpeed(180.0f)
        , m_Rotation(rotation)
        , m_FitToHeight(true)
    {
        UpdateCameraProjection();
    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        JADE_PROFILE_FUNCTION();

        // Input Handling
        // WASD for xz movement
        if (Jade::Input::IsKeyPressed(Jade::Key::A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
        }
        else if (Jade::Input::IsKeyPressed(Jade::Key::D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
        }
        if (Jade::Input::IsKeyPressed(Jade::Key::W))
        {
            m_CameraPosition.z -= m_CameraTranslationSpeed * ts;
        }
        else if (Jade::Input::IsKeyPressed(Jade::Key::S))
        {
            m_CameraPosition.z += m_CameraTranslationSpeed * ts;
        }

        // Q/E for y movement
        if (Jade::Input::IsKeyPressed(Jade::Key::Q))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
        }
        else if (Jade::Input::IsKeyPressed(Jade::Key::E))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation.z)) * m_CameraTranslationSpeed * ts;
        }

        if (m_Rotation)
        {
            // Z/X for z-axis rotation
            if (Jade::Input::IsKeyPressed(Jade::Key::Z))
                m_CameraRotation.z += m_CameraRotationSpeed * ts;
            else if (Jade::Input::IsKeyPressed(Jade::Key::X))
                m_CameraRotation.z -= m_CameraRotationSpeed * ts;

            // Wrap around rotation
            if (m_CameraRotation.z > 180.0f)
                m_CameraRotation.z -= 360.0f;
            else if (m_CameraRotation.z <= -180.0f)
                m_CameraRotation.z += 360.0f;
        }

        // Update Camera
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        JADE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(JADE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(JADE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        JADE_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

        UpdateCameraProjection();

        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        JADE_PROFILE_FUNCTION();

        if(m_FitToHeight)
            m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        else
            m_AspectRatio = (float)e.GetHeight() / (float)e.GetWidth();

        UpdateCameraProjection();

        return false;
    }
    void OrthographicCameraController::UpdateCameraProjection()
    {
        JADE_PROFILE_FUNCTION();

        if(m_FitToHeight)
            m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        else
            m_Camera.SetProjection(-m_ZoomLevel, m_ZoomLevel, -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel);
    }
}