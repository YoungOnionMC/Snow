#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#include "Snow/Platform/OpenGL/OpenGLCommon.h"

#if defined(SNOW_WINDOW_WIN32)
    #include <glad/glad_wgl.h>
#elif defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
#elif defined(SNOW_WINDOW_XLIB)
    #include <glad/glad_glx.h>
#endif

namespace Snow {
    namespace Render {
        OpenGLContext::OpenGLContext(const ContextSpecification& spec) {
            m_Specification = spec;
            SNOW_CORE_TRACE("=============================");
            SNOW_CORE_INFO("Using OpenGL Render API");


#if defined(SNOW_WINDOW_WIN32)

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

#elif defined(SNOW_WINDOW_GLFW)

            glfwMakeContextCurrent((GLFWwindow*)m_Specification.WindowHandle);
            int status = gladLoadGL();
            status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#elif defined(SNOW_WINDOW_XLIB)

            Display* display = XOpenDisplay(0);
            
            GLint attribList[] = {
                GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None
            };

            
            
            XVisualInfo* visualInfo = glXChooseVisual(display, 0, attribList);
            SNOW_CORE_TRACE("BRUH2");

            GLXContext context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
            ::Window* window = (::Window*)m_Specification.WindowHandle;
            glXMakeCurrent(display, *window, context);
            SNOW_CORE_TRACE("BRUH1");
#endif

            FindExtensions();

            SNOW_CORE_INFO("OpenGL Information:");
            SNOW_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
            SNOW_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
            SNOW_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));
            SNOW_CORE_TRACE("=============================");

            GLenum error = glGetError();

            SwapChainSpecification swapchainSpec = {};
            m_OpenGLSwapChain = static_cast<OpenGLSwapChain*>(SwapChain::CreateSwapChain(swapchainSpec));
        }

        void OpenGLContext::FindExtensions() {
            if (GLAD_GL_ARB_compute_shader) {
                SNOW_CORE_TRACE("Supports Compute shaders");
                GLint64 computeMemorySize;
                glGetInteger64v(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &computeMemorySize);

                SNOW_CORE_TRACE("Max compute shader size : {0}", computeMemorySize);
            }
        }
    }
}