#include <spch.h>
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

namespace Snow {
    namespace Render {
        namespace API {
            Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size) {
                switch(Renderer::GetRenderAPI()) {
                    case RenderAPIType::OpenGL: return Ref<OpenGLVertexBuffer>::Create(data, size);
                }
                return nullptr;
            }

            Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size) {
                switch(Renderer::GetRenderAPI()) {
                    case RenderAPIType::OpenGL: return Ref<OpenGLIndexBuffer>::Create(data, size);
                }
                return nullptr;
            }

            
        }
    }
}