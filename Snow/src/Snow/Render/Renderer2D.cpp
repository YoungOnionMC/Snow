#include <spch.h>
#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Render {
        void Renderer2D::DrawQuad(Math::Vector2f position, Math::Vector2f size) {

            float vboData[] = {
                position.x - size.x/2, position.y - size.y/2,
                position.x + size.x/2, position.y - size.y/2,
                position.x + size.x/2, position.y + size.y/2,
                position.x - size.x/2, position.y + size.y/2
                
            };

            uint32_t iboData[] = {
                0, 1, 2,
                2, 3, 0
            };

            Ref<API::VertexBuffer> vbo = API::VertexBuffer::Create(vboData, sizeof(vboData));
            Ref<API::IndexBuffer> ibo = API::IndexBuffer::Create(iboData, sizeof(iboData));

            

            // Pipeline

            PipelineSpecification pipelineSpec;
            pipelineSpec.Layout = {
                { AttribType::Float2, "Position" }
            };
            pipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/TestVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/TestFrag.glsl") };

            Ref<Pipeline> pipeline = Pipeline::Create(pipelineSpec);

            pipeline->Bind();
            vbo->Bind();
            ibo->Bind();

            Renderer::DrawIndexed(ibo->GetCount());
        }
    }
}