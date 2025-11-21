#include <Jade.h>

// ---Entry Point-----------------
#include <Jade/Core/EntryPoint.h>
// -------------------------------

#include "EditorLayer.h"

namespace Jade 
{
    class EditorApp : public Application
    {
    public:
        EditorApp()
            : Application(ApplicationSpecification{ "Jade Editor", 1600, 900 })
        {
            PushLayer(new EditorLayer());
        }
        ~EditorApp() = default;
    };

    Application* CreateApplication()
    {
        return new EditorApp();
    }
} // namespace Jade