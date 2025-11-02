#include "jdpch.h"

#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Jade
{
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::API::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
            default:
                break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}