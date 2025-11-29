#include "jdpch.h"

#include "Jade/Renderer/GraphicsContext.h"

#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Jade
{
    Scope<GraphicsContext> GraphicsContext::Create(void* windowHandle)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            JADE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(windowHandle));
        default:
            break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}  // namespace Jade
