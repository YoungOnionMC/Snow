#include <spch.h>
#include "Snow/Render/Buffer.h"

#include "Snow/Render/Renderer.h"

//#include "Snow/Platform/OpenGL/OpenGLBuffer.h"
#include "Snow/Platform/Vulkan/VulkanBuffer.h"

#if defined(SNOW_PLATFORM_WINDOWS)
    //#include "Snow/Platform/DirectX11/DirectXBuffer.h"
#endif

namespace Snow {
    namespace Render {
        Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
            switch (Render::RendererAPI::Current()) {
                //case RendererAPIType::OpenGL: return Ref<OpenGLVertexBuffer>::Create(data, size);
            case RendererAPIType::Vulkan: return Ref<VulkanVertexBuffer>::Create(size);
#if defined(SNOW_PLATFORM_WINDOWS)
                //case RendererAPIType::DirectX:    return Ref<DirectX11VertexBuffer>::Create(data, size);
#endif
            }
            return nullptr;
        }

        Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size) {
            switch (Render::RendererAPI::Current()) {
            //case RendererAPIType::OpenGL: return Ref<OpenGLVertexBuffer>::Create(data, size);
            case RendererAPIType::Vulkan: return Ref<VulkanVertexBuffer>::Create(data, size);
#if defined(SNOW_PLATFORM_WINDOWS)
            //case RendererAPIType::DirectX:    return Ref<DirectX11VertexBuffer>::Create(data, size);
#endif
            }
            return nullptr;
        }

        Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size) {
            switch (Render::RendererAPI::Current()) {
            //case RendererAPIType::OpenGL: return Ref<OpenGLIndexBuffer>::Create(data, size);
            case RendererAPIType::Vulkan: return Ref<VulkanIndexBuffer>::Create(data, size);
#if defined(SNOW_PLATFORM_WINDOWS)
            //case RendererAPIType::DirectX:    return Ref<DirectX11IndexBuffer>::Create(data, size);
#endif
            }
            return nullptr;
        }
    }
}