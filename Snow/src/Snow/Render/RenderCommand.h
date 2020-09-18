#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/API/Buffer.h"

namespace Snow {
    namespace Render {
        class RenderCommand {
        public:
            virtual void BeginScene() = 0;
            virtual void EndScene() = 0;

            virtual void DrawIndexed(uint32_t count) = 0;

            virtual void SetViewport(int width, int height) = 0;

            static RenderCommand* Create();
        };
    }
}