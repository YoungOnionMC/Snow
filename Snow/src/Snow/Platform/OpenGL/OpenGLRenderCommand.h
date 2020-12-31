#pragma once

#include "Snow/Render/RenderCommand.h"

namespace Snow {
    namespace Render {
        class OpenGLRenderCommand : public RenderAPI {
        public:
            //OpenGLRenderCommand();

            virtual void Init() override;

            void BeginCommandBuffer() override {}
            void EndCommandBuffer() override {}

            void SetClearColor(const glm::vec4& color) override;
            void Clear() override;

            void DrawIndexed(uint32_t count, PrimitiveType type) override;

            void SetBlending(bool blend) override;
            void SetDepthTesting(bool depthTest) override;

            //void ClearColorAttachment(const glm::vec4& color) override;

            void SetViewport(uint32_t width, uint32_t height) override;

            void SwapBuffers() override;

        private:

        };
    }
}