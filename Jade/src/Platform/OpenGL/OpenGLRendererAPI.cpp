#include "jdpch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Jade
{
    // OpenGL debug message callback function
    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         JADE_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:       JADE_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW:          JADE_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: JADE_CORE_TRACE(message); return;
        }

        JADE_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRendererAPI::Init()
    {
        JADE_PROFILE_FUNCTION();

#ifdef JADE_DEBUG
        // Enable OpenGL debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
                                                                     
        // Query and log the maximum number of texture image units
        {
            int maxFragUnits = 0, maxVertexUnits = 0, maxCombinedUnits = 0;

            glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragUnits);
            glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUnits);
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedUnits);

            JADE_CORE_INFO("OpenGL Texture Image Units: ");
            JADE_CORE_INFO("  GL_MAX_TEXTURE_IMAGE_UNITS (fragment): {0}", maxFragUnits);
            JADE_CORE_INFO("  GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS (vertex): {0}", maxVertexUnits);
            JADE_CORE_INFO("  GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS (all stages): {0}", maxCombinedUnits);
        }
#endif

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable depth testing for 3D rendering
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        JADE_PROFILE_FUNCTION();

        // Set the viewport for rendering
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        // Set the color used when clearing the screen
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        JADE_PROFILE_FUNCTION();

        // GL_COLOR_BUFFER_BIT and GL_DEPTH_BUFFER_BIT are bitwise ORed to clear both the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        JADE_PROFILE_FUNCTION();

        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        // Draw the elements using the index buffer
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        //// Unbind the texture after drawing
        //glBindTexture(GL_TEXTURE_2D, 0);
    }
}