#include "jdpch.h"

#include "Jade/Renderer/Renderer2D.h"
#include "Jade/Renderer/VertexArray.h"
#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jade
{
    // Vertex structure for a quad
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        glm::vec2 TilingFactor;
    };

    // Renderer2D internal data
    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        // Predefined quad vertex positions
        const glm::vec4 QuadVertexPositions[4] =
        {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };

        // Predefined quad texture coordinates
        const glm::vec2 QuadTextureCoords[4] =
        {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f }
        };

        // Culling data
        bool CullingEnabled = false;    // For testing Perspective camera, culling is off by default
        float CullingPadding = 0.0f;    // Extra padding for culling calculations
        glm::vec2 ViewMin{ 0.0f };
        glm::vec2 ViewMax{ 0.0f };

        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    // Helpers: AABB overlap and visibility test
    static inline bool AABBOverlap(const glm::vec2& aMin, const glm::vec2& aMax,
        const glm::vec2& bMin, const glm::vec2& bMax)
    {
        if (aMax.x < bMin.x || aMin.x > bMax.x) return false;
        if (aMax.y < bMin.y || aMin.y > bMax.y) return false;

        return true;
    }

    /*
    // Checks if a quad is visible within the current view bounds
    static inline bool IsVisible(const glm::vec3& center, float rotationDeg, const glm::vec2& size)
    {
        if(!s_Data.CullingEnabled)
            return true;

        const glm::vec2 halfSize = size * 0.5f;
        glm::vec2 extents = halfSize;

        // Calculate rotated extents
        if (rotationDeg != 0.0f)
        {
            const float rad = glm::radians(rotationDeg);
            const float cosTheta = glm::abs(glm::cos(rad));
            const float sinTheta = glm::abs(glm::sin(rad));
            extents.x = halfSize.x * cosTheta + halfSize.y * sinTheta;
            extents.y = halfSize.x * sinTheta + halfSize.y * cosTheta;
        }

        // Calculate quad AABB
        glm::vec2 quadMin = glm::vec2(center) - extents;
        glm::vec2 quadMax = glm::vec2(center) + extents;

        // Apply culling padding
        return AABBOverlap(quadMin, quadMax, s_Data.ViewMin, s_Data.ViewMax);
    }
    */

    // Checks if a quad defined by a transform matrix is visible within the current view bounds
    static inline bool IsVisible(const glm::mat4& transform)
    {
        if (!s_Data.CullingEnabled)
            return true;

        // Calculate quad AABB by transforming all four corners
        glm::vec4 p0 = transform * s_Data.QuadVertexPositions[0];
        glm::vec2 quadMin(p0.x, p0.y);
        glm::vec2 quadMax = quadMin;

        for (int i = 1; i < 4; ++i)
        {
            glm::vec4 pw = transform * s_Data.QuadVertexPositions[i];
            glm::vec2 pt(pw.x, pw.y);
            quadMin = glm::min(quadMin, pt);
            quadMax = glm::max(quadMax, pt);
        }

        return AABBOverlap(quadMin, quadMax, s_Data.ViewMin, s_Data.ViewMax);
    }

    void Renderer2D::Init()
    {
        JADE_PROFILE_FUNCTION();

        // Create vertex array
        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float,  "a_TexIndex" },
            { ShaderDataType::Float2, "a_TilingFactor" }
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

        // Create white texture
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; ++i)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Set all texture slots to the white texture initially
        for (int i = 0; i < s_Data.MaxTextureSlots; ++i)
        {
            s_Data.TextureSlots[i] = s_Data.WhiteTexture;
            s_Data.TextureSlots[i]->Bind(i);
        }
    }

    void Renderer2D::Shutdown()
    {
        JADE_PROFILE_FUNCTION();

        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        JADE_PROFILE_FUNCTION();

        // Set view projection matrix
        glm::mat4 viewProj = camera.GetProjectionMatrix() * glm::inverse(transform);

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", viewProj);

        // Calculate view bounds for culling
        if (s_Data.CullingEnabled)
        {
            const glm::mat4& invViewProj = glm::inverse(viewProj);

            auto toWorld = [&](const glm::vec2& ndc)
                {
                    glm::vec4 p = invViewProj * glm::vec4(ndc, 0.0f, 1.0f);
                    return glm::vec2(p) / p.w;
                };

            // Transform NDC corners to world space
            glm::vec2 c0 = toWorld({ -1.0f, -1.0f });
            glm::vec2 c1 = toWorld({ 1.0f,  -1.0f });
            glm::vec2 c2 = toWorld({ 1.0f, 1.0f });
            glm::vec2 c3 = toWorld({ -1.0f, 1.0f });

            // Compute AABB of the view
            glm::vec2 vmin = glm::min(glm::min(c0, c1), glm::min(c2, c3));
            glm::vec2 vmax = glm::max(glm::max(c0, c1), glm::max(c2, c3));

            // Apply culling padding
            const glm::vec2 pad(s_Data.CullingPadding);
            s_Data.ViewMin = vmin - pad;
            s_Data.ViewMax = vmax + pad;
        }

        // Start a new batch
        StartBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        JADE_PROFILE_FUNCTION();

        // Set view projection matrix
        glm::mat4 viewProj = camera.GetViewProjectionMatrix();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", viewProj);

        // Calculate view bounds for culling
        if (s_Data.CullingEnabled)
        {
            const glm::mat4& invViewProj = glm::inverse(viewProj);

            auto toWorld = [&](const glm::vec2& ndc)
                {
                    glm::vec4 p = invViewProj * glm::vec4(ndc, 0.0f, 1.0f);
                    return glm::vec2(p) / p.w;
                };

            // Transform NDC corners to world space
            glm::vec2 c0 = toWorld({ -1.0f, -1.0f });
            glm::vec2 c1 = toWorld({ 1.0f,  -1.0f });
            glm::vec2 c2 = toWorld({ 1.0f, 1.0f });
            glm::vec2 c3 = toWorld({ -1.0f, 1.0f });

            // Compute AABB of the view
            glm::vec2 vmin = glm::min(glm::min(c0, c1), glm::min(c2, c3));
            glm::vec2 vmax = glm::max(glm::max(c0, c1), glm::max(c2, c3));

            // Apply culling padding
            const glm::vec2 pad(s_Data.CullingPadding);
            s_Data.ViewMin = vmin - pad;
            s_Data.ViewMax = vmax + pad;
        }

        // Start a new batch
        StartBatch();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        JADE_PROFILE_FUNCTION();

        // Set view projection matrix
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // Calculate view bounds for culling
        {
            const glm::mat4& invViewProj = glm::inverse(camera.GetViewProjectionMatrix());

            auto toWorld = [&](const glm::vec2& ndc)
                {
                    glm::vec4 p = invViewProj * glm::vec4(ndc, 0.0f, 1.0f);
                    return glm::vec2(p) / p.w;
                };

            // Transform NDC corners to world space
            glm::vec2 c0 = toWorld({ -1.0f, -1.0f });
            glm::vec2 c1 = toWorld({ 1.0f,  -1.0f });
            glm::vec2 c2 = toWorld({ 1.0f, 1.0f });
            glm::vec2 c3 = toWorld({ -1.0f, 1.0f });

            // Compute AABB of the view
            glm::vec2 vmin = glm::min(glm::min(c0, c1), glm::min(c2, c3));
            glm::vec2 vmax = glm::max(glm::max(c0, c1), glm::max(c2, c3));

            // Apply culling padding
            const glm::vec2 pad(s_Data.CullingPadding);
            s_Data.ViewMin = vmin - pad;
            s_Data.ViewMax = vmax + pad;
        }

        // Start a new batch
        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        JADE_PROFILE_FUNCTION();

        Flush();
    }

    // Draws all the batched quads
    void Renderer2D::Flush()
    {
        JADE_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount == 0)
            return; // Nothing to draw

        // Bind textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }

        // Update vertex buffer data
        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }

    // Starts a new batch for rendering
    void Renderer2D::StartBatch()
    {
        // Reset batching data
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        // Reset texture slots
        s_Data.TextureSlotIndex = 1;

        // Reset stats
        s_Data.Stats.DrawCalls = 0;
        s_Data.Stats.QuadCount = 0;
    }

    // Advances to the next batch when the current one is full
    void Renderer2D::NextBatch()
    {
        Flush();

        StartBatch();
    }

#pragma region DrawQuad
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // Transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        // Transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        // Visibility culling
        if (!IsVisible(transform))
            return;

        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices || s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            NextBatch();

        constexpr float textureIndex = 0.0f;  // White Texture
        constexpr glm::vec2 tilingFactor = { 1.0f, 1.0f };

#pragma region Add vertices to buffer
        for (int i = 0; i < 4; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTextureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
#pragma endregion
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        JADE_PROFILE_FUNCTION();

        // Visibility culling
        if (!IsVisible(transform))
            return;

        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices || s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            NextBatch();

        float textureIndex = 0.0f;
        // Check if texture is already bound
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        // New texture
        if (textureIndex == 0.0f)
        {
            // Ensure we don't exceed max texture slots
            JADE_CORE_ASSERT(s_Data.TextureSlotIndex < Renderer2DData::MaxTextureSlots, "Texture slot index exceeds maximum!");

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

#pragma region Add vertices to buffer
        for (int i = 0; i < 4; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTextureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
#pragma endregion
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        // Transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawRotatedQuad(transform, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        // Transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawRotatedQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        JADE_PROFILE_FUNCTION();

        // Visibility culling
        if (!IsVisible(transform))
            return;

        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        constexpr float textureIndex = 0.0f; // White Texture
        constexpr glm::vec2 tilingFactor = { 1.0f, 1.0f };

#pragma region Add vertices to buffer
        for (int i = 0; i < 4; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTextureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
#pragma endregion
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
    {
        JADE_PROFILE_FUNCTION();

        // Visibility culling
        if (!IsVisible(transform))
            return;

        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices || s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            NextBatch();

        float textureIndex = 0.0f;
        // Check if texture is already bound
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        // New texture
        if (textureIndex == 0.0f)
        {
            // Ensure we don't exceed max texture slots
            JADE_CORE_ASSERT(s_Data.TextureSlotIndex < Renderer2DData::MaxTextureSlots, "Texture slot index exceeds maximum!");

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

#pragma region Add vertices to buffer
        for (int i = 0; i < 4; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.QuadTextureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
#pragma endregion
    }
#pragma endregion

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }

    void Renderer2D::SetCulling(bool enabled)
    {
        s_Data.CullingEnabled = enabled;
    }

    void Renderer2D::SetCullingPadding(float padding)
    {
        s_Data.CullingPadding = padding;
    }
}
