#include <spch.h>
#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Render {

        struct Vertex {
            Math::Vector3f Position;
            Math::Vector2f TexCoord;
        };

        struct Renderer2DStaticData {
            Ref<API::VertexBuffer> VBO;
            Ref<API::IndexBuffer> IBO;
            Vertex* VertexData;
            Vertex* VertexBase;

            static const uint32_t MaxQuads = 2500;
            static const uint32_t MaxVertices = MaxQuads * 4;
            static const uint32_t MaxIndicies = MaxQuads * 6;

            Ref<Pipeline> MainPipeline;


            Math::Vector2f TexCoords[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

            uint32_t IndexCount = 0;
        };
        static Renderer2DStaticData s_Data;
        


        void Renderer2D::Init() {

            s_Data.VertexBase = new Vertex[s_Data.MaxVertices];
            s_Data.VertexData = s_Data.VertexBase;

            s_Data.VBO = API::VertexBuffer::Create(nullptr, sizeof(Vertex) * s_Data.MaxVertices);

            uint32_t* indicies = new uint32_t[s_Data.MaxIndicies];
            uint32_t offset = 0;
            for(uint32_t i=0; i < s_Data.MaxQuads; i++) {
                indicies[i * 6 + 0] = 0 + offset;
                indicies[i * 6 + 1] = 1 + offset;
                indicies[i * 6 + 2] = 2 + offset;
                indicies[i * 6 + 3] = 2 + offset;
                indicies[i * 6 + 4] = 3 + offset;
                indicies[i * 6 + 5] = 0 + offset;

                offset += 4;
            }

            s_Data.IBO = API::IndexBuffer::Create(indicies, sizeof(uint32_t) * s_Data.MaxIndicies);
            delete[] indicies;
            

            // Pipeline

            PipelineSpecification pipelineSpec;
            pipelineSpec.Layout = {
                { AttribType::Float3, "Position" },
                { AttribType::Float2, "TexCoord" }
            };
            pipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/TestVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/TestFrag.glsl") };

            s_Data.MainPipeline = Pipeline::Create(pipelineSpec);

        }

        void Renderer2D::BeginBatch() {
            s_Data.VertexData = s_Data.VertexBase;
            s_Data.IndexCount = 0;
        }

        void Renderer2D::EndBatch() {
            ptrdiff_t size = (uint8_t*)s_Data.VertexData - (uint8_t*)s_Data.VertexBase;
            s_Data.VBO->Bind();
            s_Data.VBO->SetData(s_Data.VertexBase, size);
        }

        void Renderer2D::PresentBatch() {
            EndBatch();

            s_Data.MainPipeline->Bind();
            s_Data.VBO->Bind();
            s_Data.IBO->Bind();
            Renderer::DrawIndexed(s_Data.IndexCount);

            BeginBatch();
        }

        void Renderer2D::SubmitQuad(Math::Vector2f position, Math::Vector2f size) {
            
            Math::Vector3f scale[] = { {0.0f, 0.0f, 0.0f}, {size.x, 0.0f, 0.0f}, {size.x, size.y, 0.0f}, {0.0f, size.y, 0.0f} };

            for(size_t i=0; i<4; i++) {
                s_Data.VertexData->Position = Math::Vector3f(position, 0.0f) + scale[i];
                s_Data.VertexData->TexCoord = s_Data.TexCoords[i];
                s_Data.VertexData++;
            }

            s_Data.IndexCount += 6;
        }
    }
}