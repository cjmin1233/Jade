#include "jdpch.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Jade
{
    static constexpr uint32_t s_MaxFrameBufferSize = 8192;

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
        : m_Specification(spec)
        , m_RendererID(0)
        , m_ColorAttachment(0)
        , m_DepthAttachment(0)
    {
        JADE_PROFILE_FUNCTION();

        Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        JADE_PROFILE_FUNCTION();

        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        JADE_PROFILE_FUNCTION();

        // Delete existing framebuffer and attachments if they exist
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        // Create framebuffer
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // Create color attachment texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

        // Allocate storage for the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
            m_Specification.Width, m_Specification.Height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, m_ColorAttachment, 0);

        // Create depth attachment renderbuffer
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

        // Allocate storage for the renderbuffer
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8,
            m_Specification.Width, m_Specification.Height);

        // Attach the renderbuffer to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D, m_DepthAttachment, 0);

        // Check if framebuffer is complete
        JADE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
            "FrameBuffer is incomplete!");

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Bind()
    {
        JADE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        // Set the viewport to match the framebuffer size
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFrameBuffer::Unbind()
    {
        JADE_PROFILE_FUNCTION();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        JADE_PROFILE_FUNCTION();

        if(width == 0 || height == 0 ||
            width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
        {
            JADE_CORE_WARN("Attempted to resize FrameBuffer to {0}, {1}", width, height);
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }
}
