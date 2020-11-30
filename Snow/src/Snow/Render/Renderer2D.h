#pragma once

#include "Snow/Render/Renderer.h"
#include "Snow/Render/API/Buffer.h"
#include "Snow/Render/API/Texture.h"

#include "Snow/Render/Camera.h"

#include "Snow/Render/Pipeline.h"

#include <glm/glm.hpp>


namespace Snow {
    namespace Render {
        class Renderer2D {
        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(const Camera& camera, const glm::mat4& transform);
            static void EndScene();


            static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
            static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<API::Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
            static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<API::Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

            static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
            static void DrawQuad(const glm::mat4& transform, const Ref<API::Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

            static void DrawLine(const glm::vec2& startPosition, const glm::vec2& endPosition, const glm::vec4& color);
            static void DrawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec4& color);

        private:
            static void BeginBatch();
            static void EndBatch();

            static void NextBatch();
        };
    }
}