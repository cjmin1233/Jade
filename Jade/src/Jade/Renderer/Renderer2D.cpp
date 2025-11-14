#include "jdpch.h"

#include "Jade/Renderer/Renderer2D.h"
#include "Jade/Renderer/VertexArray.h"
#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jade
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        // TODO: Add texture index and tiling factor
    };

    struct Renderer2DData
    {
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        JADE_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        // Create quad index buffer
        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxQuads; ++i)
        {
            uint32_t index = i * 6;
            quadIndices[index + 0] = offset + 0;
            quadIndices[index + 1] = offset + 1;
            quadIndices[index + 2] = offset + 2;

            quadIndices[index + 3] = offset + 2;
            quadIndices[index + 4] = offset + 3;
            quadIndices[index + 5] = offset + 0;

            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

        // Delete temporary index array
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        JADE_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        JADE_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::EndScene()
    {
        JADE_PROFILE_FUNCTION();

        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    // Draws all the batched quads
    void Renderer2D::Flush()
    {
        JADE_PROFILE_FUNCTION();

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    }

#pragma region DrawQuad
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        // Add vertices to buffer
        s_Data.QuadVertexBufferPtr->Position = { position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z };
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;

        //s_Data.WhiteTexture->Bind(0);

        //glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
        //    glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        //
        //s_Data.TextureShader->SetUniformMat4("u_Transform", transform);
        //s_Data.TextureShader->SetUniformFloat2("u_TilingFactor", { 1.0f, 1.0f });
        //s_Data.TextureShader->SetUniformFloat4("u_TintColor", color);

        //s_Data.QuadVertexArray->Bind();

        //RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        JADE_PROFILE_FUNCTION();

        texture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * /* Rotation * */
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data.TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data.TextureShader->SetUniformFloat4("u_TintColor", tintColor);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec2& tilingFactor, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, tilingFactor, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec2& tilingFactor, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        s_Data.WhiteTexture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data.TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data.TextureShader->SetUniformFloat4("u_TintColor", color);

        s_Data.QuadVertexArray->Bind();

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        JADE_PROFILE_FUNCTION();

        texture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data.TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data.TextureShader->SetUniformFloat4("u_TintColor", tintColor);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
    }
#pragma endregion
}