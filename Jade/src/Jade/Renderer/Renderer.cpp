#include "jdpch.h"

#include "Jade/Renderer/Renderer.h"
#include "Jade/Renderer/Renderer2D.h"

namespace Jade
{
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        JADE_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        JADE_PROFILE_FUNCTION();

        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        JADE_PROFILE_FUNCTION();

        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::BeginScene(PerspectiveCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, 
        const Ref<VertexArray>& vertexArray,
        const glm::mat4& transform)
    {
        JADE_PROFILE_FUNCTION();

        shader->Bind();
        shader->SetUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetUniformMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}