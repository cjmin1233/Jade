#pragma once
#include "Core.h"
#include "Jade/Core/LayerStack.h"
#include "Jade/Core/Window.h"
#include "Jade/Events/Event.h"
#include "Jade/Events/ApplicationEvent.h"
#include "Jade/ImGui/ImGuiLayer.h"
#include "Jade/Renderer/Shader.h"
#include "Jade/Renderer/Buffer.h"

class Layer;

namespace Jade
{
    class JADE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline Window& GetWindow() { return *m_Window; }
        static inline Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray;
        bool m_Running;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}