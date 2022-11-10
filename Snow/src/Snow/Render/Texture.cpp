#include <spch.h>
#include "Snow/Render/Texture.h"

//#include "Snow/Platform/OpenGL/OpenGLTexture.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"

#if defined(SNOW_PLATFORM_WINDOWS)
//#include "Snow/Platform/DirectX11/DirectXTexture.h"
#endif

#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Render {
        Ref<Texture2D> Texture2D::Create(ImageFormat format, uint32_t width, uint32_t height, void* data, TextureProperties properties) {
            switch (Render::RendererAPI::Current()) {
            case RendererAPIType::None:   return nullptr;
            //case RendererAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(format, width, height, data, properties);
            case RendererAPIType::Vulkan: return Ref<VulkanTexture2D>::Create(format, width, height, data, properties);
#if defined(SNOW_PLATFORM_WINDOWS)
                //case RenderAPIType::DirectX:    return Ref<DirectX11Texture2D>::Create(format, width, height, wrap);
#endif
            }
        }

        Ref<Texture2D> Texture2D::Create(const std::string& path, TextureProperties properties) {
            switch (Render::RendererAPI::Current()) {
            case RendererAPIType::None:   return nullptr;
            //case RendererAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(path, properties);
            case RendererAPIType::Vulkan: return Ref<VulkanTexture2D>::Create(path, properties);
#if defined(SNOW_PLATFORM_WINDOWS)
                //case RenderAPIType::DirectX:    return Ref<DirectX11Texture2D>::Create(path, srgb);
#endif
            }
        }

        Ref<TextureCube> TextureCube::Create(ImageFormat format, uint32_t width, uint32_t height, const void* data, TextureProperties properties) {
            switch (Render::RendererAPI::Current()) {
            case RendererAPIType::None:   return nullptr;
            case RendererAPIType::Vulkan:   return Ref<VulkanTextureCube>::Create(format, width, height, data, properties);
            //case RendererAPIType::OpenGL: return Ref<OpenGLTextureCube>::Create(format, width, height);
            }
        }

        Ref<TextureCube> TextureCube::Create(const std::string& path, TextureProperties properties) {
            switch (Render::RendererAPI::Current()) {
            case RendererAPIType::None:   return nullptr;
            //case RendererAPIType::OpenGL: return Ref<OpenGLTextureCube>::Create(path, srgb);
            }
        }

        /*
        uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height) {
            uint32_t levels = 1;
            while ((width | height) >> levels)
                levels++;

            return levels;
        }*/
    }
}