#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLRenderCommand.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {
        OpenGLRenderCommand::OpenGLRenderCommand() {
            SNOW_CORE_TRACE("Creating OpenGL RenderCommand");
        }


        void OpenGLRenderCommand::BeginScene() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            glClearColor(0.3, 0.3, 0.3, 1.0);

            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0, 0.0); // Bottom left
            glVertex2f(-0.5, -0.5);
            glColor3f(0.0f, 1.0, 0.0); // Bottom right
            glVertex2f(0.5, -0.5);
            glColor3f(0.0f, 0.0, 1.0); // top middle
            glVertex2f(0.0, 0.5);
            glEnd();
        }

        void OpenGLRenderCommand::EndScene() {
            OpenGLContext* glContext = static_cast<OpenGLContext*>(Renderer::GetContext());
            OpenGLSwapChain& glSwapChain = glContext->GetSwapChain();

            glSwapChain.SwapBuffers();
        }

        void OpenGLRenderCommand::SetViewport(int width, int height) {
            glViewport(0, 0, width, height);
        }
    }
}