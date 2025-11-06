#include "jdpch.h"

#include "Jade/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Jade
{
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
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
        const Ref<OpenGLShader> openGLShader = std::static_pointer_cast<OpenGLShader>(shader);

        if (openGLShader)
        {
            openGLShader->Bind();
            openGLShader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
            openGLShader->UploadUniformMat4("u_Transform", transform);
        }

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}