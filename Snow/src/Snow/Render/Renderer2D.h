#pragma once

#include "Snow/Render/Renderer.h"
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Math/Math.h"


namespace Snow {
    namespace Render {
        class Renderer2D {
        public:
            static void DrawQuad(Math::Vector2f position, Math::Vector2f size);

        };
    }
}