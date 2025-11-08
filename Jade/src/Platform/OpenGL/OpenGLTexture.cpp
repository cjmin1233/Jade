#include "jdpch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>

namespace Jade
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path)
        , m_Width(0)
        , m_Height(0)
        , m_RendererID(0)
    {
        int width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        // Load image data
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        JADE_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;

        // Determine the image format based on the number of channels
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        JADE_CORE_ASSERT(internalFormat & dataFormat,
            "Format not supported!");

        // Create OpenGL texture
        // glCreateTextures: generates texture object names
        // glTextureStorage2D: allocates immutable storage for the texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        // m_RendererID 텍스처에 1 레벨, internalFormat 포맷, m_Width x m_Height 크기로 저장소 할당
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

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
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }
}