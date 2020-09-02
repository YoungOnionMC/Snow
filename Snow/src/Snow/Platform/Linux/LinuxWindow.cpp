#include <spch.h>
#include "Snow/Core/Window.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Snow {
    namespace Core {

        GLFWwindow* GLFWWindowHandle;
        int GLFWResult;

        bool Window::PlatformInit() {
            GLFWResult = glfwInit();
            if(!GLFWResult)
                SNOW_CORE_ERROR("GLFW initilization failed");

            GLFWWindowHandle = glfwCreateWindow(720, 720, "Test Window", nullptr, nullptr);
            
            return true;
        }
    }
}