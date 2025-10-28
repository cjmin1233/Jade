#include "jdpch.h"

#include "Jade/Renderer/Renderer.h"
#include "Jade/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Jade
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            JADE_CORE_ASSERT(false, "RendererAPI::None is not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexArray();
        default:
            break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}