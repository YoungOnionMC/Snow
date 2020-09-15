#include <spch.h>
#include "Snow/Core/Window.h"

#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>
#elif defined(SNOW_WINDOW_XLIB)
    #include <X11.h>
    #include <XInput.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Core {

#if defined(SNOW_WINDOW_GLFW)
        GLFWwindow* GLFWWindowHandle;
        int GLFWResult;

        void WindowCloseCallback(GLFWwindow* window) {

            auto appWindow = Application::Get().GetWindow();

            Event::WindowCloseEvent event;
            Event::EventSystem::AddEvent(event);
        }

        void WindowMinimizeCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Minimized Window");

            auto appWindow = Application::Get().GetWindow();
            Event::WindowMinimizedEvent event;
            Event::EventSystem::AddEvent(event);
            
        }

        void WindowMaximizedCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Window Maximized");
        }

        void WindowMovedCallback(GLFWwindow* window, int xPos, int yPos) {
            Event::WindowMovedEvent event(xPos, yPos);
            Event::EventSystem::AddEvent(event);
        }

        void WindowResizeCallback(GLFWwindow* window, int width, int height) {
            Event::WindowResizeEvent event(width, height);
            Event::EventSystem::AddEvent(event);
        }

        void WindowFocusCallback(GLFWwindow* window, int focus) {
            //SNOW_CORE_TRACE("Lost Focus? {0}", focus == GLFW_FALSE);
        }
#elif defined(SNOW_WINDOW_XLIB)
#endif

        bool Window::PlatformInit() {
            SNOW_CORE_TRACE("Creating Linux window");
#if defined(SNOW_WINDOW_GLFW)
            GLFWResult = glfwInit();
            if(!GLFWResult)
                SNOW_CORE_ERROR("GLFW initilization failed");
            else {
                SNOW_CORE_INFO("GLFW initialized");        
            }
                

            GLFWWindowHandle = glfwCreateWindow(720, 720, "Test Window", nullptr, nullptr);
            SNOW_CORE_INFO("Using GLFW window platform");

            glfwSetWindowCloseCallback(GLFWWindowHandle, WindowCloseCallback);
            glfwSetWindowIconifyCallback(GLFWWindowHandle, WindowMinimizeCallback);
            glfwSetWindowSizeCallback(GLFWWindowHandle, WindowResizeCallback);
            glfwSetWindowMaximizeCallback(GLFWWindowHandle, WindowMaximizedCallback);
            glfwSetWindowPosCallback(GLFWWindowHandle, WindowMovedCallback);
            glfwSetWindowFocusCallback(GLFWWindowHandle, WindowFocusCallback);
           // int yes = glfwVulkanSupported();
           // SNOW_CORE_TRACE("Vulkan Supported {0}", yes);
#elif defined(SNOW_WINDOW_XLIB)

#endif


            return true;
        }

        bool Window::PlatformShutdown() {
#if defined(SNOW_WINDOW_GLFW)
            SNOW_CORE_INFO("Destroying GLFW window");
            if(GLFWWindowHandle)
                glfwDestroyWindow(GLFWWindowHandle);
#elif defined(SNOW_WINDOW_XLIB)

#endif
            return true;
        }

        void Window::PlatformUpdate() {
#if defined(SNOW_WINDOW_GLFW)
            glfwPollEvents();
#elif defined(SNOW_WINDOW_XLIB)

#endif
        }

        void* Window::GetWindowHandle() {
#if defined(SNOW_WINDOW_GLFW)
            return GLFWWindowHandle;
#elif defined(SNOW_WINDOW_XLIB)

#endif
        }
    }
}