#pragma once
#include "Jade/Renderer/FrameBuffer.h"

namespace Jade
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);
        virtual ~OpenGLFrameBuffer();

        // (Re)creates the framebuffer and its attachments
        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
        {
            JADE_CORE_ASSERT(index < m_ColorAttachments.size(), "Color attachment index out of bounds");

            return m_ColorAttachments[index];
        }

        virtual const FrameBufferSpecification& GetSpecification() const override
        {
            return m_Specification;
        }

    private:
        uint32_t m_RendererID;

        FrameBufferSpecification m_Specification;

        // Separate color and depth attachment specifications
        // for easier access
        std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;   // Color attachment specs
        FrameBufferTextureSpecification m_DepthAttachmentSpecification;                 // Depth attachment spec

        std::vector<uint32_t> m_ColorAttachments;   // OpenGL texture IDs for color attachments
        uint32_t m_DepthAttachment;                 // OpenGL texture ID for depth attachment
    };
}
