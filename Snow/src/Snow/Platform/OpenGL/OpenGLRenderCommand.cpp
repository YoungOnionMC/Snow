#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

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

        static GLenum GetPrimitiveType(PrimitiveType type) {
            switch(type){
            case PrimitiveType::None:   return GL_NONE;
            case PrimitiveType::Triangle:   return GL_TRIANGLES;
            case PrimitiveType::Line:   return GL_LINES;
            }
        }

        void OpenGLRenderCommand::Init() {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLLogMessage, nullptr);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
        }

        void OpenGLRenderCommand::DrawIndexed(uint32_t count, PrimitiveType type) {
            glDrawElements(GetPrimitiveType(type), count, GL_UNSIGNED_INT, nullptr);
        }

        void OpenGLRenderCommand::SetClearColor(const glm::vec4& color) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void OpenGLRenderCommand::Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            
        }

        void OpenGLRenderCommand::SetViewport(uint32_t width, uint32_t height) {
            glViewport(0, 0, width, height);
        }

        void OpenGLRenderCommand::SwapBuffers() {
            OpenGLContext* glContext = static_cast<OpenGLContext*>(Render::Renderer::GetContext());
            glContext->GetSwapChain().SwapBuffers();
        }
    }
}