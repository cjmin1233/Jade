#pragma once
#include "Jade/Core/Base.h"

namespace Jade
{
    // Enum for framebuffer texture formats
    enum class FrameBufferTextureFormat
    {
        None = 0,
        // Color formats
        RGBA8,                      // 8-bit RGBA
        RED_INTEGER,                // Integer red channel

        // Depth/stencil formats
        DEPTH24STENCIL8,            // 24-bit depth + 8-bit stencil
        Depth = DEPTH24STENCIL8,    // Default depth format
        // TODO: Add more formats as needed
    };

    // Specification for a single texture attachment in the framebuffer
    struct FrameBufferTextureSpecification
    {
        FrameBufferTextureSpecification() = default;
        FrameBufferTextureSpecification(FrameBufferTextureFormat format)
            : TextureFormat(format) {}

        // Texture format of the framebuffer attachment
        FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
        // TODO: Filtering/wrap
    };

    // Specification for multiple framebuffer attachments
    struct FrameBufferAttachmentSpecification
    {
        FrameBufferAttachmentSpecification() = default;
        // Initialize with a list of texture specifications
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
            : Attachments(attachments) {}

        // List of texture attachments
        std::vector<FrameBufferTextureSpecification> Attachments;
    };

    // Overall specification for the framebuffer
    struct FrameBufferSpecification
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Samples = 1;           // Multisampling

        // Attachments specification
        FrameBufferAttachmentSpecification AttachmentSpec;

        bool SwapChainTarget = false;   // Is this framebuffer targeting the swap chain?
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

        virtual const FrameBufferSpecification& GetSpecification() const = 0;

        static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
    };
}
