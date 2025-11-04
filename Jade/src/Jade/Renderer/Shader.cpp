#include "jdpch.h"

#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Jade
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::API::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return Ref<Shader>(new OpenGLShader(filepath));
            default:
                break;
        }
        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::API::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return Ref<Shader>(new OpenGLShader(vertexSrc, fragmentSrc));
            default:
                break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}