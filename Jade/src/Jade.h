#pragma once
// For use by Jade applications
#include "Jade/Core/Base.h"
#include "Jade/Core/Application.h"
#include "Jade/Core/Log.h"
#include "Jade/Core/Layer.h"
#include "Jade/ImGui/ImGuiLayer.h"
#include "Jade/Core/Input.h"
#include "Jade/Core/KeyCodes.h"
#include "Jade/Core/Timestep.h"

#pragma region Renderer APIs
#include "Jade/Renderer/Renderer.h"
#include "Jade/Renderer/Renderer2D.h"
#include "Jade/Renderer/RenderCommand.h"
#include "Jade/Renderer/RendererAPI.h"
#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/Buffer.h"
#include "Jade/Renderer/VertexArray.h"
#include "Jade/Renderer/OrthographicCamera.h"
#include "Jade/Renderer/OrthographicCameraController.h"
#include "Jade/Renderer/Texture.h"
#pragma endregion