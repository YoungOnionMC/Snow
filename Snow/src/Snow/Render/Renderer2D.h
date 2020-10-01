#pragma once

#include "Snow/Render/Renderer.h"
#include "Snow/Render/API/Buffer.h"
#include "Snow/Render/API/Texture.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Math/Math.h"


namespace Snow {
    namespace Render {
        class Renderer2D {
        public:
            static void Init();
            static void Shutdown();



            static void PresentBatch();


            static void SubmitQuad(Math::Vector2f position, Math::Vector2f size, Math::Vector4f tint = Math::Vector4f(1.0f));

        private:
            static void BeginBatch();
            static void EndBatch();
        };
    }
}