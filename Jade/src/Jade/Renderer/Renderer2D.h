#pragma once
#include "Jade/Renderer/OrthographicCamera.h"
#include "Jade/Renderer/Texture.h"

namespace Jade
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
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
        // Rotated Primitives
        static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size, 
            const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, 
            const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));
        // Rotated Textured
        static void DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size,
            const Ref<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& tintColor = glm::vec4(1.0f));
#pragma endregion
    };
}