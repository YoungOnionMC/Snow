#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Snow {
    namespace Render {
        OpenGLContext::OpenGLContext(const ContextSpecification& spec) {
            m_Specification = spec;
            SNOW_CORE_TRACE("=============================");
            SNOW_CORE_INFO("Using OpenGL Render API");

            glfwMakeContextCurrent((GLFWwindow*)m_Specification.WindowHandle);
            int status = gladLoadGL();
            status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            SNOW_CORE_INFO("OpenGL Information:");
            SNOW_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
            SNOW_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
            SNOW_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
            SNOW_CORE_TRACE("=============================");

            SwapChainSpecification swapchainSpec = {};
            m_OpenGLSwapChain = static_cast<OpenGLSwapChain*>(SwapChain::Create(swapchainSpec));
        }
    }
}