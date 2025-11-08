#pragma once
#include "Jade/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Jade
{
    class OpenGLShader : public Shader
    {
        using GLenum = unsigned int;
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const std::string& GetName() const override { return m_Name; }

        virtual void SetUniformInt(const std::string& name, int value) override { UploadUniformInt(name, value); }

        virtual void SetUniformFloat(const std::string& name, float value) override { UploadUniformFloat(name, value); }
        virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override { UploadUniformFloat2(name, value); }
        virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override { UploadUniformFloat3(name, value); }
        virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override { UploadUniformFloat4(name, value); }

        virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override { UploadUniformMat3(name, value); }
        virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override { UploadUniformMat4(name, value); }

    private:
#pragma region Uploaders
        void UploadUniformInt(const std::string& name, int value);

        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
#pragma endregion

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

        void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}