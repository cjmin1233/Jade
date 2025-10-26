#pragma once
#include "Jade/Core/Window.h"
#include "Jade/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Jade
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override
        {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const override 
        { 
            return static_cast<void*>(m_Window); 
        }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData
        {
            std::string Title = "";
            unsigned int Width = 0;
            unsigned int Height = 0;
            bool VSync = false;
            EventCallbackFn EventCallback = nullptr;
        };

        WindowData m_Data;
    };
}