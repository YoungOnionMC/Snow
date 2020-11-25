#include <spch.h>
#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Render {

        struct QuadVertex {
            Math::Vector3f Position;
            Math::Vector2f TexCoord;
            float TexID;
            Math::Vector4f Color;
        };

        struct LineVertex {
            Math::Vector3f Position;
            Math::Vector4f Color;
        };

        struct Renderer2DStaticData {
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

            Math::Vector4f QuadVertexPositions[4];

            Ref<API::Texture2D> WhiteTexture;

            std::vector<Ref<API::Texture2D>> TextureSlots;
            uint32_t TextureSlotIndex = 1;


            Math::Vector2f TexCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };


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
                pipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/TestVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/TestFrag.glsl") };
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
                linePipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/BatchRenderLineVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/BatchRenderLineFrag.glsl") };
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

        void Renderer2D::BeginScene(const Camera& camera, const Math::Matrix4x4f& transform) {
            Math::Matrix4x4f projMatrix = camera.GetProjection();

            //s_Data.QuadPipeline->SetUniformBuffer("Camera", projMatrix * transform, sizeof(Math::Matrix4x4f));
        }

        void Renderer2D::EndScene() {
            
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

        void Renderer2D::PresentBatch() {
            //EndBatch();
            EndBatch();

            
            //s_Data.QuadVBO->Bind();
            //s_Data.QuadIBO->Bind();
            //Renderer::DrawIndexed(s_Data.IndexCount);

            BeginBatch();
        }

        void Renderer2D::DrawQuad(Math::Vector2f position, Math::Vector2f size, Math::Vector4f color) {
            DrawQuad({ position.x, position.y, 0.0f }, size, color);
        }

        void Renderer2D::DrawQuad(Math::Vector3f position, Math::Vector2f size, Math::Vector4f color) {
            Math::Matrix4x4b transform = Math::translate(Math::Matrix4x4f(1.0f), position) * Math::scale(Math::Matrix4x4f(1.0f), { size.x, size.y, 0.0f });

            DrawQuad(transform, color);
        }

        void Renderer2D::DrawQuad(const Math::Matrix4x4f& transform, const Math::Vector4f color) {
            constexpr size_t quadVertexCount = 4;
            const float textureIndex = 0.0f;
            constexpr Math::Vector2f textureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            for (size_t i = 0; i < quadVertexCount; i++) {
                s_Data.QuadVertexData->Position = transform * s_Data.QuadVertexPositions[i];
                s_Data.QuadVertexData->TexCoord = textureCoords[i];
                s_Data.QuadVertexData->TexID = textureIndex;
                s_Data.QuadVertexData->Color = color;
                s_Data.QuadVertexData++;
            }

            s_Data.QuadIndexCount += 6;
        }


        void Renderer2D::DrawLine(const Math::Vector2f& startPosition, const Math::Vector2f& endPosition, const Math::Vector4f& color) {
            DrawLine({startPosition.x, startPosition.y, 0.0f}, {endPosition.x, endPosition.y, 0.0f}, color);
        }

        void Renderer2D::DrawLine(const Math::Vector3f& startPosition, const Math::Vector3f& endPosition, const Math::Vector4f& color) {
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