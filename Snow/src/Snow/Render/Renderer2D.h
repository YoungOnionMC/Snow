#pragma once

#include "Snow/Render/Renderer.h"
#include "Snow/Render/API/Buffer.h"
#include "Snow/Render/API/Texture.h"

#include "Snow/Render/Camera.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Math/Math.h"


namespace Snow {
    namespace Render {
        class Renderer2D {
        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(const Camera& camera, const Math::Matrix4x4f& transform);
            static void EndScene();
            static void Flush();



            static void PresentBatch();


            static void DrawQuad(Math::Vector2f position, Math::Vector2f size, Math::Vector4f color);
            static void DrawQuad(Math::Vector3f position, Math::Vector2f size, Math::Vector4f color);
            static void DrawQuad(Math::Vector2f position, Math::Vector2f size, const Ref<API::Texture2D>& texture);
            static void DrawQuad(Math::Vector3f position, Math::Vector2f size, const Ref<API::Texture2D>& texture);

            static void DrawQuad(const Math::Matrix4x4f& transform, const Math::Vector4f color);
            static void DrawQuad(const Math::Matrix4x4f& transform, const Ref<API::Texture2D>& texture);

        private:
            static void BeginBatch();
            static void EndBatch();
        };
    }
}