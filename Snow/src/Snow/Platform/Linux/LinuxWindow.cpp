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
            Event::EventSystem::AddEvent(event);
            
            // Generate window close event....
        }

        void WindowMinimizeCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Minimized Window");

            auto appWindow = Application::Get().GetWindow();
            Event::WindowMinimizedEvent event;
            //Event::EventSystem::SendEvent(&event);
            
        }

        void WindowMaximizedCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Window Maximized");
        }

        void WindowMovedCallback(GLFWwindow* window, int xPos, int yPos) {
            Event::WindowMovedEvent event(xPos, yPos);
            Event::EventSystem::AddEvent(event);

            //SNOW_CORE_TRACE("Window Moved {0}, {1}", xPos, yPos);
        }

        void WindowResizeCallback(GLFWwindow* window, int width, int height) {
            //SNOW_CORE_TRACE("Resizing Window, {0}, {1}", width, height);

            auto appWindow = Application::Get().GetWindow();
            Event::WindowResizeEvent event(width, height);
            Event::EventSystem::AddEvent(event);
            //SNOW_CORE_TRACE(event.GetWidth());
            //Event::EventSystem::RegisterClient(Event::WindowResizeEvent::ID, appWindow->m_WindowResizeListener);
            //Event::EventSystem::RegisterClient(Event::WindowMinimizedEvent::ID, appWindow->m_WindowResizeListener);
            //Event::EventSystem::SendEvent(&event);
        }

        void WindowFocusCallback(GLFWwindow* window, int focus) {
            //SNOW_CORE_TRACE("Lost Focus? {0}", focus == GLFW_FALSE);
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
            glfwSetWindowMaximizeCallback(GLFWWindowHandle, WindowMaximizedCallback);
            glfwSetWindowPosCallback(GLFWWindowHandle, WindowMovedCallback);
            glfwSetWindowFocusCallback(GLFWWindowHandle, WindowFocusCallback);

           // int yes = glfwVulkanSupported();
           // SNOW_CORE_TRACE("Vulkan Supported {0}", yes);

            return true;
        }

        bool Window::PlatformShutdown() {
            if(GLFWWindowHandle)
                glfwDestroyWindow(GLFWWindowHandle);

            return true;
        }

        void Window::PlatformUpdate() {
            glfwPollEvents();
        }

        void* Window::GetWindowHandle() {
            return GLFWWindowHandle;
        }
    }
}