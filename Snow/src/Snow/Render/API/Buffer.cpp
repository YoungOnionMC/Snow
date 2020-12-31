#include <spch.h>
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

#if defined(SNOW_PLATFORM_WINDOWS)
    #include "Snow/Platform/DirectX11/DirectXBuffer.h"
#endif

namespace Snow {
    namespace Render {
        namespace API {
            Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size) {
                switch(Renderer::GetRenderAPI()) {
                    case RenderAPIType::OpenGL: return Ref<OpenGLVertexBuffer>::Create(data, size);
#if defined(SNOW_PLATFORM_WINDOWS)
                    case RenderAPIType::DirectX:    return Ref<DirectX11VertexBuffer>::Create(data, size);
#endif
                }
                return nullptr;
            }

            Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size) {
                switch(Renderer::GetRenderAPI()) {
                    case RenderAPIType::OpenGL: return Ref<OpenGLIndexBuffer>::Create(data, size);
#if defined(SNOW_PLATFORM_WINDOWS)
                    case RenderAPIType::DirectX:    return Ref<DirectX11IndexBuffer>::Create(data, size);
#endif
                }
                return nullptr;
            }

            
        }
    }
}