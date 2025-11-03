#include "jdpch.h"

#include "Jade/Renderer/Texture.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Jade
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            JADE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return Ref<Texture2D>(new OpenGLTexture2D(path));
        default:
            break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}