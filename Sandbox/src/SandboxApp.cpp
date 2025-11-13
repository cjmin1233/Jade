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
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}