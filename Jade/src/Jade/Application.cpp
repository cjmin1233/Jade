#include "Application.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace Jade
{
    Application::Application()
    {
    }
    Application::~Application()
    {
    }
    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);

        if(e.IsInCategory(EventCategoryApplication))
        {
            JADE_TRACE(e.ToString());
        }
        if(e.IsInCategory(EventCategoryInput))
        {
            JADE_TRACE(e.ToString());
            //JADE_TRACE(e);
        }

        while (true)
        {
        }
    }
} // namespace Jade