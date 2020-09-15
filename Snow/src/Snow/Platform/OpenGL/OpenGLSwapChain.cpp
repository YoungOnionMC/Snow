#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"

#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <Windows.h>
#include <glad/glad_wgl.h>

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {
        OpenGLSwapChain::OpenGLSwapChain(const SwapChainSpecification& spec):
            m_Specification(spec) {
            SNOW_CORE_TRACE("Creating OpenGL SwapChain");
        }

        void OpenGLSwapChain::SwapBuffers() {
            Core::Window* window = Core::Application::Get().GetWindow();
            auto dc = GetDC((HWND)window->GetWindowHandle());
            
            ::SwapBuffers(dc);
            //glfwSwapBuffers((GLFWwindow*)window->GetWindowHandle());


        }
    }
}
