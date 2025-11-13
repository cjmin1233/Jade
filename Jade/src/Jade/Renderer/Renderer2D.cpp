#include "jdpch.h"

#include "Jade/Renderer/Renderer2D.h"
#include "Jade/Renderer/VertexArray.h"
#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jade
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_Data;

    void Renderer2D::Init()
    {
        JADE_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[5 * 4] = 
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        };

        Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        squareVB->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
            });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformInt("u_Texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        JADE_PROFILE_FUNCTION();

        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        JADE_PROFILE_FUNCTION();

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        s_Data->WhiteTexture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        
        s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data->TextureShader->SetUniformFloat2("u_TilingFactor", { 1.0f, 1.0f });
        s_Data->TextureShader->SetUniformFloat4("u_TintColor", color);

        s_Data->QuadVertexArray->Bind();

        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
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

        s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data->TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data->TextureShader->SetUniformFloat4("u_TintColor", tintColor);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec2& tilingFactor, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, tilingFactor, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec2& tilingFactor, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        s_Data->WhiteTexture->Bind(0);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data->TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data->TextureShader->SetUniformFloat4("u_TintColor", color);

        s_Data->QuadVertexArray->Bind();

        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
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

        s_Data->TextureShader->SetUniformMat4("u_Transform", transform);
        s_Data->TextureShader->SetUniformFloat2("u_TilingFactor", tilingFactor);
        s_Data->TextureShader->SetUniformFloat4("u_TintColor", tintColor);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}