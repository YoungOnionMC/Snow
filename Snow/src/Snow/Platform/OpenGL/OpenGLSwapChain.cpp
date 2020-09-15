#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"

#include <glad/glad.h>
#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
#elif defined(SNOW_WINDOW_WIN32)
    #include <Windows.h>
    #include <glad/glad_wgl.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {
        OpenGLSwapChain::OpenGLSwapChain(const SwapChainSpecification& spec):
            m_Specification(spec) {
            SNOW_CORE_TRACE("Creating OpenGL SwapChain");
        }

        void OpenGLSwapChain::SwapBuffers() {

            Core::Window* window = Core::Application::Get().GetWindow();
#if defined(SNOW_WINDOW_WIN32)
            auto dc = GetDC((HWND)window->GetWindowHandle());
            
            ::SwapBuffers(dc);
#elif defined(SNOW_WINDOW_GLFW)
            glfwSwapBuffers((GLFWwindow*)window->GetWindowHandle());
#endif


        }
    }
}
