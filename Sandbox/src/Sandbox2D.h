#pragma once
#include <Jade.h>
#include <Jade/Core/Layer.h>

#include <glm/glm.hpp>

class Sandbox2D : public Jade::Layer
{
public:
    Sandbox2D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Jade::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Jade::Event& event) override;

private:
    Jade::Ref<Jade::Texture2D> m_Texture;
    glm::vec2 m_SquareSize;
    glm::vec2 m_TilingFactor;
    glm::vec4 m_SquareColor;
};
