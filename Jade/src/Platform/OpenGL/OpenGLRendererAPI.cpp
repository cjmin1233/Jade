#include "jdpch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Jade
{
    void OpenGLRendererAPI::Init()
    {
        JADE_PROFILE_FUNCTION();

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
    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        JADE_PROFILE_FUNCTION();

        // Draw the elements using the index buffer
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        //// Unbind the texture after drawing
        //glBindTexture(GL_TEXTURE_2D, 0);
    }
}