#include "jdpch.h"

#include "imgui.h"
#include "Jade/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Jade/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Jade
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
        , m_Time(0.0f)
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Key mapping (ImGui 1.87+ 방식)
        // GLFW 키와 ImGuiKey를 매핑하는 코드는 보통 이벤트 콜백에서 처리합니다.
        // 예시: GLFW 키 이벤트 콜백에서 아래와 같이 호출
        // ImGui::GetIO().AddKeyEvent(ImGuiKey_Tab, action == GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Tab, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_LeftArrow, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_RightArrow, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_UpArrow, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_DownArrow, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_PageUp, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_PageDown, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Home, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_End, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Insert, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Delete, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Backspace, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Space, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Enter, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Escape, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_LeftCtrl, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_LeftShift, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_LeftAlt, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_LeftSuper, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_RightCtrl, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_RightShift, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_RightAlt, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_RightSuper, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Menu, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_A, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_C, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_V, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_X, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Y, GLFW_PRESS);
        io.AddKeyEvent(ImGuiKey_Z, GLFW_PRESS);

        ImGui_ImplOpenGL3_Init();
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), 
            (float)app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
    }
}