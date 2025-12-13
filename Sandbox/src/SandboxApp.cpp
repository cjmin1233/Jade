#include <Jade.h>

// ---Entry Point-----------------
#include <Jade/Core/EntryPoint.h>
// -------------------------------

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class SandboxApp : public Jade::Application
{
public:
    SandboxApp() 
        : Jade::Application(Jade::ApplicationSpecification{ "Sandbox App", 1280, 720 })
    {
        // PushLayer(new ExampleLayer());
        PushLayer(Jade::CreateRef<Sandbox2D>());
    }
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}
