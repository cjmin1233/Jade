#pragma once
#include <Jade.h>
#include <Jade/Core/Layer.h>

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
    Jade::OrthographicCameraController m_CameraController;
};