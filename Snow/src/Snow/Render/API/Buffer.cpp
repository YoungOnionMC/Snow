#include <spch.h>
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

namespace Snow {
    namespace Render {
        namespace API {
            VertexBuffer* VertexBuffer::Create(void* data, uint32_t size) {
                switch(Renderer::GetRenderAPI()) {
                    case RenderAPI::OpenGL: return new OpenGLVertexBuffer(data, size);
                }
                return nullptr;
            }
        }
    }
}