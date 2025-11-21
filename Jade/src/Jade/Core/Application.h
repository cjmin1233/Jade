#pragma once
#include "Jade/Core/Base.h"

#include "Jade/Core/LayerStack.h"
#include "Jade/Core/Window.h"
#include "Jade/Events/Event.h"
#include "Jade/Events/ApplicationEvent.h"
#include "Jade/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

class Layer;

namespace Jade
{
    struct ApplicationSpecification
    {
        std::string Name = "Jade Application";
        uint32_t Width = 1280;
        uint32_t Height = 720;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }

        void Close();

        static inline Application& Get() { return *s_Instance; }

    private:
        void Run();

        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        float m_LastFrameTime;
        bool m_Running;
        bool m_Minimized;

    private:
        static Application* s_Instance;

        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}