#include "jdpch.h"

#include "Platform/Windows/WindowsWindow.h"
#include "Jade/Events/ApplicationEvent.h"
#include "Jade/Events/KeyEvent.h"
#include "Jade/Events/MouseEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Jade/Renderer/Renderer.h"

namespace Jade
{
    //static bool s_GLFWInitialized = false;
    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
    {
        JADE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
        : m_Window(nullptr)
        , m_Context(nullptr)
        , m_Data()
    {
        JADE_PROFILE_FUNCTION();

        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        JADE_PROFILE_FUNCTION();

        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        JADE_PROFILE_FUNCTION();

        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        JADE_CORE_INFO("Creating window {0} ({1}, {2})",
            props.Title, props.Width, props.Height);

        // Initialize GLFW if needed
        if (s_GLFWWindowCount == 0)
        {
            JADE_PROFILE_SCOPE("glfwInit");

            JADE_CORE_INFO("Initializing GLFW library");

            int success = glfwInit();
            JADE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            JADE_PROFILE_SCOPE("glfwCreateWindow");

#ifdef JADE_DEBUG
            // Enable OpenGL debug context in debug builds
            if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
            {
                JADE_PROFILE_SCOPE("glfwWindowHint");
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            }
#endif

            // Create GLFW window
            m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
                m_Data.Title.c_str(), nullptr, nullptr);
            ++s_GLFWWindowCount;
        }

        // Create OpenGL context
        m_Context = CreateScope<OpenGLContext>(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

#pragma region GLFW Callbacks
        // Set GLFW callbacks here
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                JADE_PROFILE_SCOPE("Window Resize Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                JADE_PROFILE_SCOPE("Window Close Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key,
            int scancode, int action, int mods)
            {
                JADE_PROFILE_SCOPE("Key Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event((Key)key, false);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event((Key)key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event((Key)key, true);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
            {
                JADE_PROFILE_SCOPE("Char Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event((Key)keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,
            int button, int action, int mods)
            {
                JADE_PROFILE_SCOPE("Mouse Button Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event((MouseCode)button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event((MouseCode)button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window,
            double xOffset, double yOffset)
            {
                JADE_PROFILE_SCOPE("Mouse Scroll Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                JADE_PROFILE_SCOPE("Mouse Move Callback");

                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
#pragma endregion
    }

    void WindowsWindow::Shutdown()
    {
        JADE_PROFILE_FUNCTION();

        {
            JADE_PROFILE_SCOPE("glfwDestroyWindow");

            glfwDestroyWindow(m_Window);
        }

        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0)
        {
            JADE_PROFILE_SCOPE("glfwTerminate");

            JADE_CORE_INFO("Terminating GLFW library");
            glfwTerminate();
        }
    }

    void WindowsWindow::OnUpdate()
    {
        JADE_PROFILE_FUNCTION();

        {
            JADE_PROFILE_SCOPE("glfwPollEvents");

            glfwPollEvents();
        }
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        JADE_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
