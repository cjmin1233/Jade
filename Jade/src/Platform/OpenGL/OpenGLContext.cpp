#include "jdpch.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
// #include <gl/GL.h>

namespace Jade
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        JADE_CORE_ASSERT(windowHandle, "Window handle is null!");
    }
    void OpenGLContext::Init()
    {
        JADE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        JADE_CORE_ASSERT(status, "Failed to initialize Glad!");

#ifdef JADE_DEBUG
        JADE_CORE_INFO("OpenGL Info:");
        JADE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        JADE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        JADE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
#endif

        JADE_CORE_ASSERT(GLVersion.major > 4 ||
            (GLVersion.major == 4 && GLVersion.minor >= 6),
            "Jade requires at least OpenGL version 4.6!");
    }
    void OpenGLContext::SwapBuffers()
    {
        JADE_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}