#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#if defined(SNOW_USE_GLFW)
#include <GLFW/glfw3.h>
#elif defined(SNOW_USE_WIN32)
#endif
#include <glad/glad_wgl.h>

namespace Snow {
    namespace Render {
        OpenGLContext::OpenGLContext(const ContextSpecification& spec) {
            m_Specification = spec;
            SNOW_CORE_TRACE("=============================");
            SNOW_CORE_INFO("Using OpenGL Render API");

#if defined(SNOW_USE_GLFW)

            glfwMakeContextCurrent((GLFWwindow*)m_Specification.WindowHandle);
            int status = gladLoadGL();
            status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#elif defined(SNOW_USE_WIN32)
#endif
            auto dc = GetDC((HWND)m_Specification.WindowHandle);
            HGLRC TempContext = wglCreateContext(dc);
            wglMakeCurrent(dc, TempContext);
            int status = gladLoadGL();

            status = gladLoadWGLLoader((GLADloadproc)wglGetProcAddress, dc);

            const int attribList[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 1,
                WGL_CONTEXT_MINOR_VERSION_ARB, 0,
                WGL_CONTEXT_FLAGS_ARB, 0,
                0
            };

            HGLRC RenderContext = wglCreateContextAttribsARB(dc, 0, attribList);

            if (RenderContext) {
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(TempContext);
                if (!wglMakeCurrent(dc, RenderContext)) {
                    SNOW_CORE_ERROR("Failed to make OpenGL context");
                }
            }

            wglSwapIntervalEXT(0);

            SNOW_CORE_INFO("OpenGL Information:");
            SNOW_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
            SNOW_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
            SNOW_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));
            SNOW_CORE_TRACE("=============================");

            SwapChainSpecification swapchainSpec = {};
            m_OpenGLSwapChain = static_cast<OpenGLSwapChain*>(SwapChain::Create(swapchainSpec));
        }
    }
}