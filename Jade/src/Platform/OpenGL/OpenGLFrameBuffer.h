#pragma once
#include "Jade/Renderer/FrameBuffer.h"

namespace Jade
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecification& spec);
        virtual ~OpenGLFrameBuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual uint32_t GetColorAttachmentRendererID() const override 
        { 
            return m_ColorAttachment; 
        }
        virtual const FrameBufferSpecification& GetSpecification() const override 
        { 
            return m_Specification; 
        }

    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment, m_DepthAttachment;
        FrameBufferSpecification m_Specification;
    };
}