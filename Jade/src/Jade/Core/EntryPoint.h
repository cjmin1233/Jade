#pragma once
#include "Jade/Core/Core.h"

#ifdef JADE_PLATFORM_WINDOWS

extern Jade::Application* Jade::CreateApplication();

int main(int argc, char** argv)
{
    Jade::Log::Init();
    JADE_CORE_WARN("Initialized Log!");

    Jade::Application* app = Jade::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif  // JADE_PLATFORM_WINDOWS