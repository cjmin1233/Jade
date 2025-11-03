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

        // Create OpenGL texture
        // glCreateTextures: generates texture object names
        // glTextureStorage2D: allocates immutable storage for the texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

        // Set texture parameters
        // glTextureParameteri: sets texture parameters
        // GL_TEXTURE_MIN_FILTER: texture minifying function
        // GL_TEXTURE_MAG_FILTER: texture magnification function
        // Here we use GL_LINEAR for minification and GL_NEAREST for magnification
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // glTextureSubImage2D: specifies a two-dimensional texture subimage
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height,
            channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

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