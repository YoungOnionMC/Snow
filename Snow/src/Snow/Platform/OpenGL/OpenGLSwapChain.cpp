#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {
        OpenGLSwapChain::OpenGLSwapChain(const SwapChainSpecification& spec):
            m_Specification(spec) {
            SNOW_CORE_TRACE("Creating OpenGL SwapChain");
        }

        void OpenGLSwapChain::SwapBuffers() {
            Core::Window* window = Core::Application::Get().GetWindow();
            
            glfwSwapBuffers((GLFWwindow*)window->GetWindowHandle());


        }
    }
}
