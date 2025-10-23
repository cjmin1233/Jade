#include "jdpch.h"

#include "imgui.h"
#include "Jade/ImGui/ImGuiLayer.h"
#include "Jade/Core/Application.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Jade
{
    //static ImGuiKey Key::KeyCodeToImGuiKey(Key::KeyCode keycode);
    //static bool Key::IsNamedKey(ImGuiKey key);
    //static bool Key::GetKeyDown(ImGuiIO& io, ImGuiKey key);

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
        JADE_INFO("ImGuiLayer Attached");

        // ImGui 컨텍스트 생성 및 초기화
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Key mapping (ImGui 1.87+ 방식)
        // GLFW 키와 ImGuiKey를 매핑하는 코드는 보통 이벤트 콜백에서 처리합니다.
        // 예시: GLFW 키 이벤트 콜백에서 아래와 같이 호출
        // ImGui::GetIO().AddKeyEvent(ImGuiKey_Tab, action == GLFW_PRESS);

        ImGui_ImplOpenGL3_Init();
    }

    void ImGuiLayer::OnDetach()
    {
        JADE_INFO("ImGuiLayer Detached");
    }

    void ImGuiLayer::OnUpdate()
    {
        // ImGui 프레임 시작
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        // 화면 크기 설정
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), 
            (float)app.GetWindow().GetHeight());

        // 델타 타임 계산
        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        // 새 ImGui 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // 여기서 ImGui 위젯을 생성합니다.
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        // ImGui 렌더링
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        JADE_TRACE("ImGuiLayer Event Received: {0}", event.ToString());

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseButtonPressedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

        dispatcher.Dispatch<KeyPressedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

        dispatcher.Dispatch<WindowResizeEvent>(JADE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[event.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(event.GetX(), event.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += event.GetXOffset();
        io.MouseWheel += event.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        Key::KeyCode keycode = e.GetKeyCode();
        ImGuiKey imguiKey = Key::KeyCodeToImGuiKey(keycode);
        // 일반 키 입력 처리
        if (Key::IsNamedKey(imguiKey))
        {
            io.KeysData[imguiKey - ImGuiKey_NamedKey_BEGIN].Down = true;
            io.AddKeyEvent(imguiKey, true);
        }

        io.KeyCtrl = Key::GetKeyDown(io, ImGuiKey_LeftCtrl) || Key::GetKeyDown(io, ImGuiKey_RightCtrl);
        io.KeyShift = Key::GetKeyDown(io, ImGuiKey_LeftShift) || Key::GetKeyDown(io, ImGuiKey_RightShift);
        io.KeyAlt = Key::GetKeyDown(io, ImGuiKey_LeftAlt) || Key::GetKeyDown(io, ImGuiKey_RightAlt);
        io.KeySuper = Key::GetKeyDown(io, ImGuiKey_LeftSuper) || Key::GetKeyDown(io, ImGuiKey_RightSuper);

        io.AddKeyEvent(ImGuiMod_Ctrl, io.KeyCtrl);
        io.AddKeyEvent(ImGuiMod_Shift, io.KeyShift);
        io.AddKeyEvent(ImGuiMod_Alt, io.KeyAlt);
        io.AddKeyEvent(ImGuiMod_Super, io.KeySuper);

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        Key::KeyCode keycode = event.GetKeyCode();
        ImGuiKey imguiKey = Key::KeyCodeToImGuiKey(keycode);
        // 일반 키 입력 처리
        if(Key::IsNamedKey(imguiKey))
        {
            io.KeysData[imguiKey - ImGuiKey_NamedKey_BEGIN].Down = false;
            io.AddKeyEvent(imguiKey, false);
        }

        io.KeyCtrl = Key::GetKeyDown(io, ImGuiKey_LeftCtrl) || Key::GetKeyDown(io, ImGuiKey_RightCtrl);
        io.KeyShift = Key::GetKeyDown(io, ImGuiKey_LeftShift) || Key::GetKeyDown(io, ImGuiKey_RightShift);
        io.KeyAlt = Key::GetKeyDown(io, ImGuiKey_LeftAlt) || Key::GetKeyDown(io, ImGuiKey_RightAlt);
        io.KeySuper = Key::GetKeyDown(io, ImGuiKey_LeftSuper) || Key::GetKeyDown(io, ImGuiKey_RightSuper);

        io.AddKeyEvent(ImGuiMod_Ctrl, io.KeyCtrl); 
        io.AddKeyEvent(ImGuiMod_Shift, io.KeyShift);
        io.AddKeyEvent(ImGuiMod_Alt, io.KeyAlt);
        io.AddKeyEvent(ImGuiMod_Super, io.KeySuper);

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        Key::KeyCode keycode = event.GetKeyCode();

        if((int)keycode > 0 && (int)keycode < 0x10000)
            io.AddInputCharacter((ImWchar)keycode);

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        float width = (float)event.GetWidth(), height = (float)event.GetHeight();
        io.DisplaySize = ImVec2(width, height);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        glViewport(0, 0, width, height);

        return false;
    }
}