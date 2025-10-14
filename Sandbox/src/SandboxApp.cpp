#include <Jade.h>

class SandboxApp : public Jade::Application
{
public:
    SandboxApp() {}
    ~SandboxApp() {}
};

Jade::Application* Jade::CreateApplication()
{
    return new SandboxApp();
}