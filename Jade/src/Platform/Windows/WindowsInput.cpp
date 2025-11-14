#include "jdpch.h"

#include "Platform/Windows/WindowsInput.h"
#include "Jade/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Jade
{
    bool WindowsInput::IsKeyPressedImpl(Key keyCode)
    {
        // Windows-specific key press detection logic
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        int state = glfwGetKey(window, static_cast<int>(keyCode));

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(Mouse button)
    {
        // Windows-specific mouse button press detection logic
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        int state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        // Windows-specific mouse position retrieval logic
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x, (float)y };
    }

    float WindowsInput::GetMouseXImpl()
    {
        // Windows-specific mouse X position retrieval logic
        return GetMousePositionImpl().first;
    }

    float WindowsInput::GetMouseYImpl()
    {
        // Windows-specific mouse Y position retrieval logic
        return GetMousePositionImpl().second;
    }
}