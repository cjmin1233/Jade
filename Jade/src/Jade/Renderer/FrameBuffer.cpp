#include "jdpch.h"

#include "Jade/Renderer/FrameBuffer.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Jade
{
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLFrameBuffer>(spec);
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}