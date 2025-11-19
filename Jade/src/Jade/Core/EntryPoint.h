#pragma once
#include "Jade/Core/Base.h"

#ifdef JADE_PLATFORM_WINDOWS

extern Jade::Application* Jade::CreateApplication();

int main(int argc, char** argv)
{
    Jade::Log::Init();

    JADE_PROFILE_BEGIN_SESSION("Startup", "profiles/JadeProfile-Startup.json");
    Jade::Application* app = Jade::CreateApplication();
    JADE_PROFILE_END_SESSION();

    JADE_PROFILE_BEGIN_SESSION("Runtime", "profiles/JadeProfile-Runtime.json");
    app->Run();
    JADE_PROFILE_END_SESSION();

    JADE_PROFILE_BEGIN_SESSION("Shutdown", "profiles/JadeProfile-Shutdown.json");
    delete app;
    JADE_PROFILE_END_SESSION();
    return 0;
}

#endif  // JADE_PLATFORM_WINDOWS