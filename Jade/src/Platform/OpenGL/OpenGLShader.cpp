#include "jdpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Jade/Core/Timer.h"

namespace Jade
{
    namespace Utils
    {
        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            JADE_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
            case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }

            JADE_CORE_ASSERT(false, "Unknown GL shader stage!");
            return (shaderc_shader_kind)0;
        }

        static const char* GLShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }

            JADE_CORE_ASSERT(false, "Unknown GL shader stage!");
            return nullptr;
        }

        static const char* GetCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }

        static void CreateCacheDirectoryIfNeeded()
        {
            std::string cacheDirectory = GetCacheDirectory();

            if (!std::filesystem::exists(cacheDirectory))
            {
                std::filesystem::create_directories(cacheDirectory);
            }
        }

        static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return ".cached_opengl.vert";
            case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
            }

            JADE_CORE_ASSERT(false, "Unknown GL shader stage!");
            return nullptr;
        }

        static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
        {
            switch (stage)
            {
            case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
            case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
            }

            JADE_CORE_ASSERT(false, "Unknown GL shader stage!");
            return nullptr;
        }
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_RendererID(0)
        , m_FilePath(filepath)
        , m_Name("")
        , m_VulkanSPIRV{}
        , m_OpenGLSPIRV{}
        , m_OpenGLSourceCode{}
    {
        JADE_PROFILE_FUNCTION();

        Utils::CreateCacheDirectoryIfNeeded();

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        //CompileShader(shaderSources);

        {
            Timer timer;
            // Compile to Vulkan SPIR-V
            CompileOrGetVulkanBinaries(shaderSources);
            CompileOrGetOpenGLBinaries();
            CreateProgram();
            JADE_CORE_INFO("OpenGL shader creation took {0} ms", timer.ElapsedMillis());
        }

        // Extract name from filepath
        size_t lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        size_t lastDot = filepath.rfind('.');
        size_t count = lastDot == std::string::npos ?
            filepath.size() - lastSlash :
            lastDot - lastSlash;

        m_Name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc,
        const std::string& fragmentSrc)
        : m_RendererID(0)
        , m_Name(name)
    {
        JADE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER] = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;

        //CompileShader(shaderSources);

        CompileOrGetVulkanBinaries(shaderSources);
        CompileOrGetOpenGLBinaries();
        CreateProgram();
    }

    OpenGLShader::~OpenGLShader()
    {
        JADE_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        JADE_PROFILE_FUNCTION();

        // glUseProgram sets the current active shader program
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        JADE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

#pragma region Uniform Uploads
    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
#pragma endregion

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        JADE_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            // Get size of file
            size_t size = in.tellg();

            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                JADE_CORE_ERROR("Could not read from file \"{0}\"", filepath);
            }
        }
        else
        {
            JADE_CORE_ERROR("Could not open file \"{0}\"", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        JADE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        // Start of shader type declaration line
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            // End of shader type declaration line
            size_t eol = source.find_first_of("\r\n", pos);
            JADE_CORE_ASSERT(eol != std::string::npos,
                "Syntax error: No end of line after shader type declaration!");

            // Start of shader type name (after "#type " keyword)
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            JADE_CORE_ASSERT(Utils::ShaderTypeFromString(type),
                "Invalid shader type specified!");

            // Start of shader code after shader type declaration line
            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            JADE_CORE_ASSERT(nextLinePos != std::string::npos,
                "Syntax error: No shader code after shader type declaration!");

            // Find next shader type declaration line
            pos = source.find(typeToken, nextLinePos);

            shaderSources[Utils::ShaderTypeFromString(type)] =
                (pos == std::string::npos) ?
                source.substr(nextLinePos) :
                source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        JADE_PROFILE_FUNCTION();

        m_RendererID = glCreateProgram();

        JADE_CORE_ASSERT(shaderSources.size() <= 2,
            "Jade only supports 2 shaders for now (vertex and fragment)!");
        size_t glShaderIDIndex = 0;
        std::array<GLenum, 2> glShaderIDs;

        for (const auto& [type, source] : shaderSources)
        {
            // Create an empty shader handle
            GLuint shader = glCreateShader(type);

            // Send the shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // Compile the shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            // Check for compilation errors
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                // Use the infoLog as you see fit.
                JADE_CORE_ERROR("{0}", infoLog.data());
                JADE_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }

            // Attach the compiled shader to the program
            glAttachShader(m_RendererID, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Link our program
        glLinkProgram(m_RendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);

            // Use the infoLog as you see fit.
            JADE_CORE_ERROR("{0}", infoLog.data());
            JADE_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id : glShaderIDs)
        {
            glDetachShader(m_RendererID, id);
            glDeleteShader(id);
        }
    }

    /// <summary>
    /// Compiles given GLSL shader sources to Vulkan SPIR-V binaries
    /// or retrieves them from cache.
    /// </summary>
    /// <param name="shaderSources">Shader source code mapped by stage.</param>
    void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        GLuint program = glCreateProgram();

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        // Set target environment to Vulkan 1.2
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        const bool optimize = true;
        if (optimize)
        {
            // Set optimization level to performance
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
        }

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

        auto& shaderData = m_VulkanSPIRV;
        shaderData.clear();
        for (auto&& [stage, source] : shaderSources)
        {
            std::filesystem::path shaderFilePath = m_FilePath;
            // Cached SPIR-V binary path
            std::filesystem::path cachedPath = cacheDirectory /
                (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            // Try to read from cached file
            if (in.is_open())
            {
                // Read cached SPIR-V binary
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);
                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)(data.data()), size);
            }
            // Compile GLSL to SPIR-V
            else
            {
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                    Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);

                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    JADE_CORE_ERROR(module.GetErrorMessage());
                    JADE_CORE_ASSERT(false);
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        // Reflect shader
        for (auto&& [stage, spirv] : shaderData)
        {
            Reflect(stage, spirv);
        }
    }

    /// <summary>
    /// Compiles Vulkan SPIR-V binaries to OpenGL SPIR-V binaries
    /// </summary>
    void OpenGLShader::CompileOrGetOpenGLBinaries()
    {
        auto& shaderData = m_OpenGLSPIRV;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        const bool optimize = false;
        if (optimize)
        {
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
        }

        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

        shaderData.clear();
        m_OpenGLSourceCode.clear();
        for (auto&& [stage, spirv] : m_VulkanSPIRV)
        {
            std::filesystem::path shaderFilePath = m_FilePath;
            std::filesystem::path cachedPath = cacheDirectory /
                (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

            std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
            // Try to read from cached file
            if (in.is_open())
            {
                // Read cached SPIR-V binary
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)(data.data()), size);
            }
            // Compile Vulkan SPIR-V to OpenGL SPIR-V
            else
            {
                spirv_cross::CompilerGLSL glslCompiler(spirv);
                m_OpenGLSourceCode[stage] = glslCompiler.compile();
                auto& source = m_OpenGLSourceCode[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                    Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    JADE_CORE_ERROR(module.GetErrorMessage());
                    JADE_CORE_ASSERT(false);
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }
    }

    void OpenGLShader::CreateProgram()
    {
        GLuint program = m_RendererID = glCreateProgram();

        std::vector<GLuint> shaderIDs;
        for (auto&& [stage, spirv] : m_OpenGLSPIRV)
        {
            // Create shader object
            GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));

            glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (int)spirv.size() * sizeof(uint32_t));
            glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
            glAttachShader(program, shaderID);
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        // Check for linking errors
        if (isLinked == GL_FALSE)
        {
            // Get the length of the info log
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            JADE_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

            glDeleteProgram(program);

            for (auto id : shaderIDs)
            {
                glDeleteShader(id);
            }
            return;
        }

        // Detach and delete shaders after successful linking
        for (auto id : shaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
    }

    /// <summary>
    /// Reflects SPIR-V shader data to extract information about
    /// uniform buffers and their properties.
    /// </summary>
    /// <param name="stage"> The shader stage (vertex, fragment, etc.). </param>
    /// <param name="shaderData"> The SPIR-V data for the shader. </param>
    void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
    {
        spirv_cross::Compiler compiler(shaderData);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        JADE_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
        JADE_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
        JADE_CORE_TRACE("    {0} resources", resources.sampled_images.size());

        JADE_CORE_TRACE("Uniform Buffers:");
        for (const auto& resource : resources.uniform_buffers)
        {
            const auto& bufferType = compiler.get_type(resource.type_id);
            uint32_t size = static_cast<uint32_t>(compiler.get_declared_struct_size(bufferType));
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int memberCount = static_cast<int>(bufferType.member_types.size());

            JADE_CORE_TRACE("  {0}", resource.name);
            JADE_CORE_TRACE("    Size = {0}", size);
            JADE_CORE_TRACE("    Binding = {0}", binding);
            JADE_CORE_TRACE("    Members = {0}", memberCount);
        }
    }
}
