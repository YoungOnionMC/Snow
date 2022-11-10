#pragma once

#include "Snow/Math/AABB.h"
#include "Snow/Render/Buffer.h"
#include "Snow/Render/Mesh.h"
#include "Snow/Render/RenderCommandBuffer.h"
//#include "Snow/Render/Renderer.h"
#include "Snow/Render/RenderPass.h"
#include "Snow/Render/Pipeline.h"
#include "Snow/Render/Texture.h"
#include "Snow/Render/UniformBufferSet.h"

//#include "Snow/Render/Camera.h"
//#include "Snow/Editor/EditorCamera.h"

#include "Snow/Render/Pipeline.h"


#include <glm/glm.hpp>


namespace Snow {
    namespace Render {
        class Renderer2D : public RefCounted {
        public:
            Renderer2D();
            virtual ~Renderer2D();

            void Init();
            void Shutdown();

            void BeginScene(const glm::mat4& viewProj, const glm::mat4& viewMatrix, bool depthTest = true);
            void EndScene();

            Ref<RenderPass> GetTargetRenderPass();
            void SetTargetRenderPass(Ref<RenderPass> renderPass);


            void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
            void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
            void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
            void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

            void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
            void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

            void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
            void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
            void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));
            void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f));

            void DrawCircle(const glm::vec3& p0, const glm::vec3& rotation, float radius, const glm::vec4& color = glm::vec4(1.0f));
            void FillCircle(const glm::vec2& p0, float radius, const glm::vec4& color, float thickness = 0.05f);
            void FillCircle(const glm::vec3& p0, float radius, const glm::vec4& color, float thickness = 0.05f);

            void DrawLine(const glm::vec2& startPosition, const glm::vec2& endPosition, const glm::vec4& color);
            void DrawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec4& color);

            void DrawAABB(const Core::AABB& aabb, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
            void DrawAABB(Ref<Mesh> mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));

            void DrawString(const std::string& text, const std::string fontName, const glm::vec2& position, const glm::vec3& color = glm::vec3(1.0f));
        
            void SetLineWidth(float lineWidth);

            struct Statistics {
                uint32_t DrawCalls = 0;
                uint32_t QuadCount = 0;
                uint32_t LineCount = 0;

                uint32_t GetTotalVertexCount() { return QuadCount * 4 + LineCount * 2; }
                uint32_t GetTotalIndexCount() { return QuadCount * 6 + LineCount * 2; }
            };
            void ResetStats();
            Statistics GetStats();
        private:
            void FlushAndReset();

            Ref<RenderCommandBuffer> m_CommandBuffer;

            static const uint32_t MaxQuads = 200000;
            static const uint32_t MaxVertices = MaxQuads * 4;
            static const uint32_t MaxIndices = MaxQuads * 6;
            static const uint32_t MaxTextureSlots = 32;

            static const uint32_t MaxLines = 10000;
            static const uint32_t MaxLineVertices = MaxLines * 4;
            static const uint32_t MaxLineIndices = MaxLines * 6;

            struct QuadVertex {
                glm::vec3 Position;
                glm::vec2 TexCoord;
                float TexID;
                glm::vec4 Color;
            };

            Ref<VertexBuffer> m_QuadVertexBuffer;
            Ref<IndexBuffer> m_QuadIndexBuffer;
            Ref<Material> m_QuadMaterial;

            uint32_t m_QuadIndexCount = 0;
            QuadVertex* m_QuadVertexBufferBase = nullptr;
            QuadVertex* m_QuadVertexBufferPtr = nullptr;

            glm::vec4 m_QuadVertexPositions[4];

            struct LineVertex {
                glm::vec3 Position;
                glm::vec4 Color;
            };

            Ref<VertexBuffer> m_LineVertexBuffer;
            Ref<IndexBuffer> m_LineIndexBuffer;
            Ref<Material> m_LineMaterial;

            uint32_t m_LineIndexCount = 0;
            LineVertex* m_LineVertexBufferBase = nullptr;
            LineVertex* m_LineVertexBufferPtr = nullptr;

            struct CircleVertex {
                glm::vec3 WorldPosition;
                float Thickness;
                glm::vec2 LocalPosition;
                glm::vec4 Color;
            };

            Ref<VertexBuffer> m_CircleVertexBuffer;
            Ref<IndexBuffer> m_CircleIndexBuffer;
            Ref<Material> m_CircleMaterial;

            uint32_t m_CircleIndexCount = 0;
            CircleVertex* m_CircleVertexBufferBase = nullptr;
            CircleVertex* m_CircleVertexBufferPtr = nullptr;

            Ref<Pipeline> m_QuadPipeline;
            Ref<Pipeline> m_LinePipeline;
            Ref<Pipeline> m_CirclePipeline;
            Ref<Pipeline> m_TextPipeline;

            Ref<Texture2D> m_WhiteTexture;

            std::array<Ref<Texture2D>, MaxTextureSlots> m_TextureSlots;
            uint32_t m_TextureSlotIndex = 1;

            glm::mat4 m_CameraViewProj;
            glm::mat4 m_CameraView;

            bool m_DepthTest = true;

            float m_LineWidth = 1.0f;

            Statistics m_Stats;

            Ref<UniformBufferSet> m_UniformBufferSet;

            struct UBCamera {
                glm::mat4 ViewProj;
            };
        };
    }
}