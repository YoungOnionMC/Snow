#include <spch.h>
#include "Snow/Core/Window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Core {

        GLFWwindow* GLFWWindowHandle;
        int GLFWResult;

        void WindowCloseCallback(GLFWwindow* window) {
            SNOW_CORE_TRACE("Closing Window");

            auto appWindow = Application::Get().GetWindow();

            Event::WindowCloseEvent event;
            
            Event::EventSystem::Get().SendEvent(event);
            
            // Generate window close event....
        }

        void WindowMinimizeCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Minimized Window");

            auto appWindow = Application::Get().GetWindow();
            Event::WindowMinimizedEvent event;
            Event::EventSystem::Get().SendEvent(event);
            
        }

        void WindowResizeCallback(GLFWwindow* window, int width, int height) {
            SNOW_CORE_TRACE("Resizing Window");

            auto appWindow = Application::Get().GetWindow();
            Event::WindowResizeEvent event(width, height);
            Event::EventSystem::Get().SendEvent(event);
            Event::EventSystem::Get().RegisterClient(event, appWindow->m_WindowResizeListener);
        }

        bool Window::PlatformInit() {
            SNOW_CORE_TRACE("=============================");
            GLFWResult = glfwInit();
            if(!GLFWResult)
                SNOW_CORE_ERROR("GLFW initilization failed");
            else {
                SNOW_CORE_INFO("GLFW initialized");        
            }
                

            GLFWWindowHandle = glfwCreateWindow(720, 720, "Test Window", nullptr, nullptr);
            SNOW_CORE_INFO("Using GLFW window platform");
            SNOW_CORE_TRACE("=============================");

            glfwSetWindowCloseCallback(GLFWWindowHandle, WindowCloseCallback);
            glfwSetWindowIconifyCallback(GLFWWindowHandle, WindowMinimizeCallback);
            glfwSetWindowSizeCallback(GLFWWindowHandle, WindowResizeCallback);


            return true;
        }

        bool Window::PlatformShutdown() {
            if(GLFWWindowHandle)
                glfwDestroyWindow(GLFWWindowHandle);

            
        }

        void Window::PlatformUpdate() {
            glfwPollEvents();
        }

        void* Window::GetWindowHandle() {
            return GLFWWindowHandle;
        }
    }
}