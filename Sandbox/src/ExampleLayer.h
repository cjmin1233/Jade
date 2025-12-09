#pragma once
#include <Jade.h>

class ExampleLayer : public Jade::Layer
{
public:
    ExampleLayer();


    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(Jade::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Jade::Event& event) override;

private:
    Jade::ShaderLibrary m_ShaderLibrary;

    Jade::Ref<Jade::VertexArray> m_TriangleVA;
    Jade::Ref<Jade::VertexArray> m_SquareVA;

    Jade::Ref<Jade::Texture2D> m_Texture;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
    float m_SquareScale = 0.1f;
};
