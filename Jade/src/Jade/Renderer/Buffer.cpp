#include "jdpch.h"

#include "Jade/Core/Log.h"
#include "Jade/Renderer/Buffer.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Jade
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            JADE_CORE_ASSERT(false,
                "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
            break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            JADE_CORE_ASSERT(false,
                "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
            break;
        }
        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}