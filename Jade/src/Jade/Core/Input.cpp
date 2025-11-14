#include "jdpch.h"

#include "Jade/Core/Input.h"

#ifdef JADE_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsInput.h"
#endif

namespace Jade
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
#ifdef JADE_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#else
        JADE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}