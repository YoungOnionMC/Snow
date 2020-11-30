#include <spch.h>
#include "Snow/Render/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Snow {
    namespace Render {

        struct QuadVertex {
            glm::vec3 Position;
            glm::vec2 TexCoord;
            float TexID;
            glm::vec4 Color;
        };

        struct LineVertex {
            glm::vec3 Position;
            glm::vec4 Color;
        };

        struct Renderer2DStaticData {
            static const uint32_t MaxTextureSlots = 32;

            static const uint32_t MaxQuads = 2500;
            static const uint32_t MaxQuadVertices = MaxQuads * 4;
            static const uint32_t MaxQuadIndicies = MaxQuads * 6;

            static const uint32_t MaxLines = 10000;
            static const uint32_t MaxLineVertices = MaxLines * 2;
            static const uint32_t MaxLineIndices = MaxLines * 6;

            uint32_t QuadIndexCount = 0;
            QuadVertex* QuadVertexData = nullptr;
            QuadVertex* QuadVertexBase = nullptr;

            Ref<Pipeline> QuadPipeline;
            Ref<API::IndexBuffer> QuadIBO;
            Ref<API::VertexBuffer> QuadVBO;

            glm::vec4 QuadVertexPositions[4];

            Ref<API::Texture2D> WhiteTexture;

            std::vector<Ref<API::Texture2D>> TextureSlots;
            uint32_t TextureSlotIndex = 1;


            glm::vec2 TexCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };


            Ref<Pipeline> LinePipeline;
            Ref<API::VertexBuffer> LineVBO;
            Ref<API::IndexBuffer> LineIBO;

            uint32_t LineIndexCount = 0;
            LineVertex* LineVertexData = nullptr;
            LineVertex* LineVertexBase = nullptr;
        };
        static Renderer2DStaticData s_Data;
        


        void Renderer2D::Init() {
            
            // Quad Pipeline
            {
                PipelineSpecification pipelineSpec;
                pipelineSpec.Layout = {
                    { AttribType::Float3, "Position" },
                    { AttribType::Float2, "TexCoord" },
                    { AttribType::Float, "TexID" },
                    { AttribType::Float4, "Color" },
                };
                pipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/QuadBatchRenderVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/QuadBatchRenderFrag.glsl") };
                pipelineSpec.Type = PrimitiveType::Triangle;

                s_Data.QuadPipeline = Pipeline::Create(pipelineSpec);

                s_Data.QuadVertexBase = new QuadVertex[s_Data.MaxQuadVertices];
                s_Data.QuadVertexData = s_Data.QuadVertexBase;

                s_Data.QuadVBO = API::VertexBuffer::Create(nullptr, sizeof(QuadVertex) * s_Data.MaxQuadVertices);

                uint32_t* indicies = new uint32_t[s_Data.MaxQuadIndicies];
                uint32_t offset = 0;
                for (uint32_t i = 0; i < s_Data.MaxQuads; i++) {
                    indicies[i * 6 + 0] = 0 + offset;
                    indicies[i * 6 + 1] = 1 + offset;
                    indicies[i * 6 + 2] = 2 + offset;
                    indicies[i * 6 + 3] = 2 + offset;
                    indicies[i * 6 + 4] = 3 + offset;
                    indicies[i * 6 + 5] = 0 + offset;

                    offset += 4;
                }

                s_Data.QuadIBO = API::IndexBuffer::Create(indicies, sizeof(uint32_t) * s_Data.MaxQuadIndicies);
                delete[] indicies;

                s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
                s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
                s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
                s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
            }

            s_Data.WhiteTexture = API::Texture2D::Create(API::TextureFormat::RGBA, 1, 1);
            uint32_t whiteTextureData = 0xffffffff;
            s_Data.WhiteTexture->Lock();
            s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
            s_Data.WhiteTexture->Unlock();

            s_Data.TextureSlots.resize(32);
            s_Data.TextureSlots[0] = s_Data.WhiteTexture;

            // Line Pipeline
            {
                PipelineSpecification linePipelineSpec;
                linePipelineSpec.Layout = {
                    { AttribType::Float3, "Position" },
                    { AttribType::Float4, "Color" }
                };
                linePipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/LineBatchRenderVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/LineBatchRenderFrag.glsl") };
                linePipelineSpec.Type = PrimitiveType::Line;

                s_Data.LinePipeline = Pipeline::Create(linePipelineSpec);

                s_Data.LineVertexBase = new LineVertex[s_Data.MaxLineVertices];
                s_Data.LineVertexData = s_Data.LineVertexBase;

                s_Data.LineVBO = API::VertexBuffer::Create(nullptr, s_Data.MaxLineVertices * sizeof(LineVertex));

                uint32_t* indices = new  uint32_t[s_Data.MaxLineIndices];
                for(uint32_t i=0; i<s_Data.MaxLineIndices; i++)
                    indices[i] = i;

                s_Data.LineIBO = API::IndexBuffer::Create(indices, s_Data.MaxLineIndices);
                delete[] indices;
            }
        }

        void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
            s_Data.QuadPipeline->Bind();

            glm::mat4 viewProjMatrix = camera.GetProjection() * glm::inverse(transform);
            s_Data.QuadPipeline->SetUniformBufferData(0, glm::value_ptr(viewProjMatrix), sizeof(glm::mat4));

            glm::vec3 color = { 0.5, 0.0, 0.8 };
            s_Data.QuadPipeline->SetUniformBufferData(1, glm::value_ptr(color), sizeof(glm::vec3));

            BeginBatch();
        }

        void Renderer2D::EndScene() {
            EndBatch();
        }

        void Renderer2D::BeginBatch() {
            s_Data.QuadVertexData = s_Data.QuadVertexBase;
            s_Data.QuadIndexCount = 0;

            s_Data.LineVertexData = s_Data.LineVertexBase;
            s_Data.LineIndexCount = 0;

            s_Data.TextureSlotIndex = 1;
        }

        void Renderer2D::EndBatch() {
            ptrdiff_t size = (uint8_t*)s_Data.QuadVertexData - (uint8_t*)s_Data.QuadVertexBase;
            if(size) {
                s_Data.QuadVBO->SetData(s_Data.QuadVertexBase, size);

                for(uint32_t i=0; i< s_Data.TextureSlotIndex; i++)
                    s_Data.TextureSlots[i]->Bind(i);
                
                s_Data.QuadPipeline->Bind();

                for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
                    s_Data.TextureSlots[i]->Bind(i);

                s_Data.QuadIBO->Bind();
                RenderCommand::DrawIndexed(s_Data.QuadIndexCount, s_Data.QuadPipeline->GetSpecification().Type);
            }

            size = (uint8_t*)s_Data.LineVertexData - (uint8_t*)s_Data.LineVertexBase;
            if(size) {
                s_Data.LineVBO->SetData(s_Data.LineVertexBase, size);
                s_Data.LinePipeline->Bind();
                s_Data.LineIBO->Bind();
                RenderCommand::DrawIndexed(s_Data.LineIndexCount, s_Data.LinePipeline->GetSpecification().Type);
            }
        }

        void Renderer2D::NextBatch() {
            EndBatch();

            BeginBatch();
        }

        void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
            DrawQuad({ position.x, position.y, 0.0f }, size, color);
        }

        void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

            DrawQuad(transform, color);
        }

        void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<API::Texture2D>& texture, const glm::vec4& tint) {
            DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint);
        }

        void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<API::Texture2D>& texture, const glm::vec4& tint) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

            DrawQuad(transform, texture, tint);
        }

        void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
            constexpr size_t quadVertexCount = 4;
            const float textureIndex = 0.0f;
            constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            if (s_Data.QuadIndexCount >= Renderer2DStaticData::MaxQuadIndicies)
                NextBatch();

            for (size_t i = 0; i < quadVertexCount; i++) {
                s_Data.QuadVertexData->Position = transform * s_Data.QuadVertexPositions[i];
                s_Data.QuadVertexData->TexCoord = textureCoords[i];
                s_Data.QuadVertexData->TexID = textureIndex;
                s_Data.QuadVertexData->Color = color;
                s_Data.QuadVertexData++;
            }

            s_Data.QuadIndexCount += 6;
        }

        void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<API::Texture2D>& texture, const glm::vec4& tint) {
            constexpr size_t quadVertexCount = 4;
            constexpr glm::vec2 textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            float textureIndex = 0.0f;
            for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
                if (s_Data.TextureSlots[i].Raw() == texture.Raw()) {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == 0.0f) {
                if (s_Data.TextureSlotIndex >= Renderer2DStaticData::MaxTextureSlots)
                    NextBatch();

                textureIndex = (float)s_Data.TextureSlotIndex;
                s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
                s_Data.TextureSlotIndex++;
            }

            for (size_t i = 0; i < quadVertexCount; i++) {
                s_Data.QuadVertexData->Position = transform * s_Data.QuadVertexPositions[i];
                s_Data.QuadVertexData->TexCoord = textureCoords[i];
                s_Data.QuadVertexData->TexID = textureIndex;
                s_Data.QuadVertexData->Color = tint;
                s_Data.QuadVertexData++;
            }

            s_Data.QuadIndexCount += 6;
        }


        void Renderer2D::DrawLine(const glm::vec2& startPosition, const glm::vec2& endPosition, const glm::vec4& color) {
            DrawLine({startPosition.x, startPosition.y, 0.0f}, {endPosition.x, endPosition.y, 0.0f}, color);
        }

        void Renderer2D::DrawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec4& color) {
            s_Data.LineVertexData->Position = startPosition;
            s_Data.LineVertexData->Color = color;
            s_Data.LineVertexData++;

            s_Data.LineVertexData->Position = endPosition;
            s_Data.LineVertexData->Color = color;
            s_Data.LineVertexData++;

            s_Data.LineIndexCount += 2;

        }
    }
}