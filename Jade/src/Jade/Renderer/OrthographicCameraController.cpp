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
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        // Input Handling
        // WASD for xz movement
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::W))
            m_CameraPosition.z -= m_CameraTranslationSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::S))
            m_CameraPosition.z += m_CameraTranslationSpeed * ts;
        // Q/E for y movement
        if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::Q))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::E))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            // Z/X for z-axis rotation
            if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::Z))
                m_CameraRotation.z += m_CameraRotationSpeed * ts;
            else if (Jade::Input::IsKeyPressed(Jade::Key::KeyCode::X))
                m_CameraRotation.z -= m_CameraRotationSpeed * ts;
        }

        // Update Camera
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(JADE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(JADE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}