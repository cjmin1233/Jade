#pragma once
#include "jdpch.h"

#include "Jade/Core/Core.h"
#include "Jade/Events/Event.h"

namespace Jade
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Jade Engine",
            unsigned int width = 1280,
            unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    // Interface representing a desktop system based Window
    class JADE_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void OnUpdate() PURE;

        virtual unsigned int GetWidth() const PURE;
        virtual unsigned int GetHeight() const PURE;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) PURE;

        virtual void SetVSync(bool enabled) PURE;
        virtual bool IsVSync() const PURE;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}