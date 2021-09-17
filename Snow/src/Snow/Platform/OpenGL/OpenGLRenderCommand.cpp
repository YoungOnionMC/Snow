#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"
#include "Snow/Platform/OpenGL/OpenGLCommon.h"
#include "Snow/Platform/OpenGL/OpenGLBuffer.h"
#include "Snow/Platform/OpenGL/OpenGLShader.h"

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
            switch(type) {
            case PrimitiveType::None:   return GL_NONE;
            case PrimitiveType::Triangle:   return GL_TRIANGLES;
            case PrimitiveType::Line:   return GL_LINES;
            }
        }

        void OpenGLRenderCommand::Init() {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLLogMessage, nullptr);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }

        void OpenGLRenderCommand::DrawIndexed(uint32_t count, PrimitiveType type) {
            GLCheckError(glDrawElements(GetPrimitiveType(type), count, GL_UNSIGNED_INT, nullptr));
        }

        void OpenGLRenderCommand::SetClearColor(const glm::vec4& color) {
            GLCheckError(glClearColor(color.r, color.g, color.b, color.a));
        }

        void OpenGLRenderCommand::Clear() {
            GLCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
            
        }

        void OpenGLRenderCommand::SetViewport(uint32_t width, uint32_t height) {
            GLCheckError(glViewport(0, 0, width, height));
        }

        void OpenGLRenderCommand::SetBlending(bool blend) {
            if (blend) {
                GLCheckError(glEnable(GL_BLEND));
                GLCheckError(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            }
            else
                GLCheckError(glDisable(GL_BLEND));
        }

        void OpenGLRenderCommand::SetDepthTesting(bool depthTest) {
            if (depthTest) {
                GLCheckError(glEnable(GL_DEPTH_TEST));
            }
            else {
                GLCheckError(glDisable(GL_DEPTH_TEST));
            }

        }

        void OpenGLRenderCommand::SetImagePacking(uint32_t size) {
            GLCheckError(glPixelStorei(GL_UNPACK_ALIGNMENT, size));
        }

        void OpenGLRenderCommand::DispatchComputeShader(Ref<Shader>& computeShader, uint32_t xGroup, uint32_t yGroup, uint32_t zGroup) {
            auto glShader = computeShader.As<OpenGLShader>();
            bool computeFound = false;
            uint32_t computeRendererID = 0;
            for (uint32_t i = 0; i < glShader->GetTypes().size(); i++) {
                if (glShader->GetTypes()[i] == ShaderType::Compute) {
                    computeFound = true;
                }
            }
            SNOW_CORE_ASSERT(computeFound, "Compute shader not in current shader");
            

            GLCheckError(glDispatchCompute(xGroup, yGroup, zGroup));

            
        }

        void OpenGLRenderCommand::SwapBuffers() {
            //OpenGLContext* glContext = static_cast<OpenGLContext*>(Render::Renderer::GetContext());
            //glContext->GetSwapChain().SwapBuffers();
        }
    }
}