#pragma once
#include "Jade/Renderer/Camera/OrthographicCamera.h"
#include "Jade/Renderer/Texture.h"

#include "Jade/Renderer/Camera/Camera.h"

namespace Jade
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const OrthographicCamera& camera);   // TODO: Remove this overload later
        static void EndScene();
        static void Flush();

#pragma region DrawQuad
        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        // Textured
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));

        // Single Quad with transform matrix
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));

        // Rotated Primitives
        static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
        // Rotated Textured
        static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));

        // Rotated Quad with transform matrix
        static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::mat4& transform,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));
#pragma endregion

        // Stats
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };
        static void ResetStats();
        static Statistics GetStats();

        // Culling controls
        static void SetCulling(bool enabled);
        static void SetCullingPadding(float padding);

    private:
        static void StartBatch();
        static void NextBatch();
    };
}
