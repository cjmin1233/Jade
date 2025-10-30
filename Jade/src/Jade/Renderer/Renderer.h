#pragma once
#include "Jade/Renderer/RenderCommand.h"
#include "Jade/Renderer/OrthographicCamera.h"
#include "Jade/Renderer/PerspectiveCamera.h"
#include "Jade/Renderer/Shader.h"

namespace Jade
{
    class Renderer
    {
    public:
        static void BeginScene(OrthographicCamera& camera);
        static void BeginScene(PerspectiveCamera& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
}