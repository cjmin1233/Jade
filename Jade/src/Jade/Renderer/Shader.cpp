#include "jdpch.h"

#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Jade
{
#pragma region Shader
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::API::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(filepath);
            default:
                break;
        }
        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                JADE_CORE_ASSERT(false, "RendererAPI::API::None is not supported");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
            default:
                break;
        }

        JADE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
#pragma endregion

#pragma region ShaderLibrary
    /// <summary>
    /// 지정한 이름으로 셰이더를 셰이더 라이브러리에 추가합니다.
    /// 동일한 이름의 셰이더가 이미 존재하면 어설션을 발생시킵니다.
    /// </summary>
    /// <param name="name">라이브러리에 추가할 셰이더의 이름.</param>
    /// <param name="shader">라이브러리에 추가할 셰이더의 참조.</param>
    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        JADE_CORE_ASSERT(!Exists(name),
            "Shader already exists in the library!");
        m_Shaders[name] = shader;
    }

    /// <summary>
    /// 셰이더를 셰이더 라이브러리에 추가합니다.
    /// </summary>
    /// <param name="shader">라이브러리에 추가할 셰이더의 참조.</param>
    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const std::string& name = shader->GetName();
        Add(name, shader);
    }

    /// <summary>
    /// 지정한 파일 경로에서 셰이더를 생성하여 라이브러리에 추가하고 해당 셰이더에 대한 참조를 반환합니다.
    /// </summary>
    /// <param name="name">라이브러리에 등록할 셰이더의 이름입니다.</param>
    /// <param name="filepath">로드할 셰이더 파일의 경로.</param>
    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        Ref<Shader> shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    /// <summary>
    /// 지정한 파일 경로에서 셰이더를 생성하여 라이브러리에 추가하고 해당 셰이더에 대한 참조를 반환합니다.
    /// </summary>
    /// <param name="filepath">로드할 셰이더 파일의 경로.</param>
    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        Ref<Shader> shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    /// <summary>
    /// 라이브러리에서 지정된 이름의 셰이더를 가져옵니다.
    /// 이름이 존재하지 않으면 어설션을 발생시킵니다.
    /// </summary>
    /// <param name="name">검색할 셰이더의 이름.</param>
    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        JADE_CORE_ASSERT(Exists(name),
            "Shader not found in the library!");
        return m_Shaders[name];
    }

    /// <summary>
    /// 지정된 이름의 셰이더가 라이브러리에 존재하는지 확인합니다.
    /// </summary>
    /// <param name="name">검색할 셰이더의 이름입니다.</param>
    /// <returns>해당 이름의 셰이더가 존재하면 true, 존재하지 않으면 false입니다.</returns>
    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }
#pragma endregion
}
