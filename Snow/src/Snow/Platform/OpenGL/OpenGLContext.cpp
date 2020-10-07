#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

#if defined(SNOW_WINDOW_WIN32)
    #include <glad/glad_wgl.h>
#elif defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
#elif defined(SNOW_WINDOW_XLIB)
    #include <glad/glad_glx.h>
#endif

namespace Snow {
    namespace Render {
        static void GLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
            if (severity == GL_DEBUG_SEVERITY_HIGH) {
                SNOW_CORE_ERROR("[OpenGL Error] - {0}", message);
            }
            else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
                SNOW_CORE_WARN("[OpenGL Error] - {0}", message);
            }
            else if (severity == GL_DEBUG_SEVERITY_LOW) {
                SNOW_CORE_INFO("[OpenGL Error] - {0}", message);
            }
            else {
                SNOW_CORE_TRACE("[OpenGL Error] - {0}", message);
            }
        }

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

            SNOW_CORE_INFO("OpenGL Information:");
            SNOW_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
            SNOW_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
            SNOW_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));
            SNOW_CORE_TRACE("=============================");

            glDebugMessageCallback(GLLogMessage, nullptr);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            GLenum error = glGetError();

            SwapChainSpecification swapchainSpec = {};
            m_OpenGLSwapChain = static_cast<OpenGLSwapChain*>(SwapChain::Create(swapchainSpec));
        }
    }
}