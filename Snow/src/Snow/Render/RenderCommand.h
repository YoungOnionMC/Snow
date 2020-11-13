#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Math/Vector/Vector4.h"

namespace Snow {
    namespace Render {
        class RenderCommand {
        public:
            virtual void BeginScene() = 0;
            virtual void EndScene() = 0;

            virtual void ClearColorAttachment(const Math::Vector4f& color) = 0;

            virtual void DrawIndexed(uint32_t count, PrimitiveType type) = 0;

            virtual void SetViewport(int width, int height) = 0;

            static RenderCommand* Create();
        };
    }
}