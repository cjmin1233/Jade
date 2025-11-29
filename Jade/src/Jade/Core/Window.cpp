#include "jdpch.h"

#include "Jade/Core/Window.h"

#ifdef JADE_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Jade
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef JADE_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
#else
        JADE_CORE_ASSERT(false, "Unknown platforms!");
        return nullptr;
#endif // JADE_PLATFORM_WINDOWS
    }
}
