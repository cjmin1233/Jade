#pragma once
#include "Core.h"

namespace Jade
{
    class JADE_API Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}