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
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        JADE_CORE_ASSERT(status, "Failed to initialize Glad!");

        JADE_CORE_INFO("OpenGL Info:");
        JADE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        JADE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        JADE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

#ifdef JADE_ENABLE_ASSERTS
        int versionMajor, versionMinor;

        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        JADE_CORE_ASSERT(versionMajor > 4 ||
            (versionMajor == 4 && versionMinor >= 6),
            "Jade requires at least OpenGL version 4.6!");
#endif // JADE_ENABLE_ASSERTS

    }
    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}