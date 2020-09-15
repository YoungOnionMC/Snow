#pragma once

#include "Snow/Render/RenderCommand.h"

namespace Snow {
    namespace Render {
        class OpenGLRenderCommand : public RenderCommand {
        public:
            OpenGLRenderCommand();

            void BeginScene() override;
            void EndScene() override;
            void SetViewport(int width, int height) override;

        private:

        };
    }
}