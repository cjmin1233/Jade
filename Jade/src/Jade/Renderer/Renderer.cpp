#include "jdpch.h"

#include "Jade/Renderer/Renderer.h"

namespace Jade
{
    void Renderer::BeginScene()
    {

    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}