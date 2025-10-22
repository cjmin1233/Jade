#include "Jade.h"

class ExampleLayer : public Jade::Layer
{
public:
    ExampleLayer()
        : Layer("ExampleLayer") {}

    void OnAttach() override
    {
        JADE_INFO("ExampleLayer Attached");
    }

    void OnDetach() override
    {
        JADE_INFO("ExampleLayer Detached");
    }

    void OnUpdate() override
    {
        // Update logic here
        // JADE_INFO("ExampleLayer Updating");
    }

    void OnEvent(Jade::Event& event) override
    {
        JADE_TRACE("ExampleLayer Event Received: {0}", event.ToString());
    }
};

class SandboxApp : public Jade::Application
{
public:
    SandboxApp() 
    {
        // PushLayer(new ExampleLayer());
        PushOverlay(new Jade::ImGuiLayer());
    }
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}