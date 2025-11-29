#pragma once

namespace Jade
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Scope<GraphicsContext> Create(void* windowHandle);
    };
}
