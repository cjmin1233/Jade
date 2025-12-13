#include "jdpch.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Jade
{
    static constexpr uint32_t s_MaxFrameBufferSize = 8192;

    namespace Utils
    {
        // Map FrameBufferTextureFormat to OpenGL internal format
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        // Create OpenGL textures
        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        // Bind OpenGL texture
        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        // Attach color texture to framebuffer
        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;

            if (multisampled)
            {
                // Allocate storage for multisampled texture
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                // Allocate storage for regular texture
                // parameters: target, level, internalformat, width, height, border, format, type, data
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                // Set texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            // Attach the texture to the framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        // Attach depth texture to framebuffer
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format,
            GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                // Allocate storage for multisampled depth texture
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                // Allocate storage for regular depth texture
                // parameters: target, levels, internalformat, width, height
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                // Set texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            // Attach the texture to the framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        // Check if the format is a depth format
        static bool IsDepthFormat(const FrameBufferTextureFormat& format)
        {
            switch (format)
            {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    return true;
            }

            return false;
        }

        // Map FrameBufferTextureFormat to OpenGL base format
        static GLenum JadeFBTextureFormatToOpenGLBaseFormat(FrameBufferTextureFormat format)
        {
            switch (format)
            {
            case FrameBufferTextureFormat::RGBA8:        return GL_RGBA;
            case FrameBufferTextureFormat::RED_INTEGER:  return GL_RED_INTEGER;
            }

            JADE_CORE_ASSERT(false, "Unknown FrameBufferTextureFormat!");
            return 0;
        }
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
        : m_RendererID(0)
        , m_Specification(spec)
        , m_ColorAttachmentSpecifications()
        , m_DepthAttachmentSpecification(FrameBufferTextureFormat::None)    // Depth spec initialized to None
        , m_ColorAttachments()
        , m_DepthAttachment(0)
    {
        JADE_PROFILE_FUNCTION();

        for (const auto& spec : m_Specification.AttachmentSpec.Attachments)
        {
            // Separate color and depth attachment specifications
            if (!Utils::IsDepthFormat(spec.TextureFormat))
            {
                m_ColorAttachmentSpecifications.emplace_back(spec);
            }
            else
            {
                m_DepthAttachmentSpecification = spec;
            }
        }

        Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        JADE_PROFILE_FUNCTION();

        glDeleteFramebuffers(1, &m_RendererID);

        glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        JADE_PROFILE_FUNCTION();

        // Delete existing framebuffer and attachments if they exist
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        // Create framebuffer
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisampled = m_Specification.Samples > 1;

        // Create color attachments
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            // Create textures for color attachments
            Utils::CreateTextures(multisampled, m_ColorAttachments.data(), (uint32_t)m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
            {
                Utils::BindTexture(multisampled, m_ColorAttachments[i]);

                // Attach texture based on its format
                switch (m_ColorAttachmentSpecifications[i].TextureFormat)
                {
                    case FrameBufferTextureFormat::RGBA8:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples,
                            GL_RGBA8, GL_RGBA,              // internal format and data format
                            m_Specification.Width, m_Specification.Height, (int)i);
                        break;
                    case FrameBufferTextureFormat::RED_INTEGER:
                        Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples,
                            GL_R32I, GL_RED_INTEGER,        // internal format and data format
                            m_Specification.Width, m_Specification.Height, (int)i);
                        break;
                }
            }
        }

        // Create depth attachment
        if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None)
        {
            // Create texture for depth attachment
            Utils::CreateTextures(multisampled, &m_DepthAttachment, 1);
            Utils::BindTexture(multisampled, m_DepthAttachment);

            // Attach texture based on its format
            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples,
                        GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,   // format and attachment type
                        m_Specification.Width, m_Specification.Height);
                    break;
            }
        }

        // Specify the list of color attachments to draw to
        if (m_ColorAttachments.size() > 1)
        {
            JADE_CORE_ASSERT(m_ColorAttachments.size() <= 4,
                "Jade only supports up to 4 color attachments!");

            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                                 GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

            // Set the draw buffers for the framebuffer
            glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

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

        // Unbind the framebuffer (bind to default framebuffer)
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

    void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        JADE_PROFILE_FUNCTION();

        JADE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(),
            "Attachment index out of bounds!");

        auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

        // Clear the specified attachment with the given value
        // parameters: texture, level, format, type, data
        glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
            Utils::JadeFBTextureFormatToOpenGLBaseFormat(spec.TextureFormat),
            GL_INT, &value);
    }

    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        JADE_PROFILE_FUNCTION();

        JADE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(),
            "Attachment index out of bounds!");

        // Set the read buffer to the specified color attachment
        // parameter: GLenum specifying which color attachment to read from
        glReadBuffer(GL_COLOR_ATTACHMENT0 + (GLenum)attachmentIndex);

        // Determine format of the target color attachment
        const auto& texSpec = m_ColorAttachmentSpecifications[attachmentIndex];

        if (texSpec.TextureFormat == FrameBufferTextureFormat::RED_INTEGER)
        {
            int pixelData = 0;
            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
            return pixelData;
        }
        else
        {
            // Assume RGBA8 or compatible; read as unsigned bytes and return R component
            unsigned char pixel[4] = { 0, 0, 0, 0 };
            glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
            return static_cast<int>(pixel[0]); // R component
        }
    }
}
