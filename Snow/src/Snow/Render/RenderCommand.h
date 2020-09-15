#pragma once

namespace Snow {
    namespace Render {
        class RenderCommand {
        public:
            virtual void BeginScene() = 0;
            virtual void EndScene() = 0;

            virtual void SetViewport(int width, int height) = 0;

            static RenderCommand* Create();
        };
    }
}