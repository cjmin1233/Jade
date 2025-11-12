#include "jdpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Jade
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
        : m_Path("")
        , m_Width(width)
        , m_Height(height)
        , m_RendererID(0)
        , m_InternalFormat(GL_RGBA8)    // 4 channels
        , m_DataFormat(GL_RGBA)         // 4 channels
    {
        JADE_PROFILE_FUNCTION();

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path)
        , m_Width(0)
        , m_Height(0)
        , m_RendererID(0)
        , m_InternalFormat(0)
        , m_DataFormat(0)
    {
        JADE_PROFILE_FUNCTION();

        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);

        stbi_uc* data = nullptr;
        {
            JADE_PROFILE_SCOPE("stbi_load");
            // Load image data
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
            JADE_CORE_ASSERT(data, "Failed to load image!");
        }
        m_Width = width;
        m_Height = height;

        // Determine the image format based on the number of channels
        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }

        JADE_CORE_ASSERT(m_InternalFormat & m_DataFormat,
            "Format not supported!");

        // Create OpenGL texture
        // glCreateTextures: generates texture object names
        // glTextureStorage2D: allocates immutable storage for the texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        // m_RendererID 텍스처에 1 레벨, internalFormat 포맷, m_Width x m_Height 크기로 저장소 할당
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        // Set texture parameters
        // glTextureParameteri: sets texture parameters
        // GL_TEXTURE_MIN_FILTER: texture minifying function
        // GL_TEXTURE_MAG_FILTER: texture magnification function
        // Here we use GL_LINEAR for minification and GL_NEAREST for magnification
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // GL_TEXTURE_WRAP_S: wrapping mode for texture coordinate S
        // GL_TEXTURE_WRAP_T: wrapping mode for texture coordinate T
        // Here we use GL_REPEAT for both S and T coordinates -> texture repeats when coordinates are outside [0,1]
        // If you want to clamp the texture coordinates instead of repeating -> GL_CLAMP_TO_EDGE
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // m_RendererID 텍스처의 0번 레벨, (0,0) 위치에서 m_Width x m_Height 영역에 dataFormat 포맷의 data를 복사
        // GL_UNSIGNED_BYTE: data의 각 색상 성분이 부호 없는 바이트 형식임을 나타냄
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        JADE_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        JADE_PROFILE_FUNCTION();

        // Calculate bytes per pixel based on data format
        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

        JADE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        JADE_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }
}