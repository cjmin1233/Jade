#pragma once

#ifdef JADE_PLATFORM_WINDOWS

extern Jade::Application* Jade::CreateApplication();

int main(int argc, char** argv)
{
    Jade::Application* app = Jade::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif  // JADE_PLATFORM_WINDOWS