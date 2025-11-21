#pragma once
#include <Jade.h>
#include <Jade/Core/Layer.h>

#include <glm/glm.hpp>

namespace Jade 
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;

    private:
        void RenderViewport();

    private:
        OrthographicCameraController m_CameraController;

        Ref<Texture2D> m_Texture;
        glm::vec2 m_SquareSize;
        glm::vec2 m_TilingFactor;
        glm::vec4 m_SquareColor;

        Ref<FrameBuffer> m_FrameBuffer;
        glm::vec2 m_ViewportSize;
    };
} // namespace Jade