#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {
        OpenGLRenderCommand::OpenGLRenderCommand() {
            SNOW_CORE_TRACE("Creating OpenGL RenderCommand");
        }


        void OpenGLRenderCommand::BeginScene() {

        }

        void OpenGLRenderCommand::EndScene() {
            OpenGLContext* glContext = static_cast<OpenGLContext*>(Renderer::GetContext());
            OpenGLSwapChain& glSwapChain = glContext->GetSwapChain();

            glSwapChain.SwapBuffers();
        }

        static GLenum GetPrimitiveType(PrimitiveType type) {
            switch(type){
            case PrimitiveType::None:   return GL_NONE;
            case PrimitiveType::Triangle:   return GL_TRIANGLES;
            case PrimitiveType::Line:   return GL_LINES;
            }
        }

        void OpenGLRenderCommand::DrawIndexed(uint32_t count, PrimitiveType type) {
            glDrawElements(GetPrimitiveType(type), count, GL_UNSIGNED_INT, nullptr);
        }

        void OpenGLRenderCommand::ClearColorAttachment(const Math::Vector4f& color) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void OpenGLRenderCommand::SetViewport(int width, int height) {
            glViewport(0, 0, width, height);
        }
    }
}