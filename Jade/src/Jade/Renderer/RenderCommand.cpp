#include "jdpch.h"

#include "Jade/Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Jade
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}