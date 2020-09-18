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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            glClearColor(0.3, 0.3, 0.3, 1.0);

        }

        void OpenGLRenderCommand::EndScene() {
            OpenGLContext* glContext = static_cast<OpenGLContext*>(Renderer::GetContext());
            OpenGLSwapChain& glSwapChain = glContext->GetSwapChain();

            glSwapChain.SwapBuffers();
        }

        void OpenGLRenderCommand::DrawIndexed(uint32_t count = 0) {
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }

        void OpenGLRenderCommand::SetViewport(int width, int height) {
            glViewport(0, 0, width, height);
        }
    }
}