#include <spch.h>
#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Render {

        struct Vertex {
            Math::Vector2f Position;
            Math::Vector2f TexCoord;
        };

        struct Renderer2DStaticData {
            Ref<API::VertexBuffer> VBO;
            Ref<API::IndexBuffer> IBO;
            Vertex* VertexData;
            Vertex* VertexBase;

            static const uint32_t MaxQuads = 2500;
            static const uint32_t MaxVerticies = MaxQuads * 4;
            static const uint32_t MaxIndicies = MaxQuads * 6;
        };
        static Renderer2DStaticData s_Data;
        


        void Renderer2D::Init() {

            s_Data.VBO = API::VertexBuffer::Create(nullptr, sizeof(Vertex) * s_Data.MaxVerticies);

            uint32_t* indicies = new uint32_t[s_Data.MaxIndicies];
            uint32_t offset = 0;
            for(uint32_t i=0; i < s_Data.MaxQuads; i++) {
                indicies[i * 6 + 0] = 0 + offset;
                indicies[i * 6 + 1] = 1 + offset;
                indicies[i * 6 + 2] = 2 + offset;
                indicies[i * 6 + 3] = 2 + offset;
                indicies[i * 6 + 4] = 3 + offset;
                indicies[i * 6 + 5] = 0 + offset;

                offset +=4;
            }

            s_Data.IBO = API::IndexBuffer::Create(indicies, sizeof(uint32_t) * s_Data.MaxIndicies);
            delete[] indicies;
            

            // Pipeline

            PipelineSpecification pipelineSpec;
            pipelineSpec.Layout = {
                { AttribType::Float2, "Position" }
            };
            pipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/TestVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/TestFrag.glsl") };

            Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpec);

        }
    }
}