#pragma once

#include "Snow/Render/RenderCommand.h"

namespace Snow {
    namespace Render {
        class OpenGLRenderCommand : public RenderCommand {
        public:
            OpenGLRenderCommand();

            void BeginScene() override;
            void EndScene() override;

            void DrawIndexed(uint32_t count, PrimitiveType type) override;

            void ClearColorAttachment(const Math::Vector4f& color) override;

            void SetViewport(int width, int height) override;

        private:

        };
    }
}