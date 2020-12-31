#include <spch.h>
#include "Snow/Render/API/Texture.h"

#include "Snow/Platform/OpenGL/OpenGLTexture.h"

#if defined(SNOW_PLATFORM_WINDOWS)
#include "Snow/Platform/DirectX11/DirectXTexture.h"
#endif

#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Render {
        namespace API {
            Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap) {
                switch(Render::Renderer::GetRenderAPI()) {
                case RenderAPIType::None:   return nullptr;
                case RenderAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(format, width, height, wrap);
#if defined(SNOW_PLATFORM_WINDOWS)
                case RenderAPIType::DirectX:    return Ref<DirectX11Texture2D>::Create(format, width, height, wrap);
#endif
                }
            }

            Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb) {
                switch(Render::Renderer::GetRenderAPI()) {
                case RenderAPIType::None:   return nullptr;
                case RenderAPIType::OpenGL: return Ref<OpenGLTexture2D>::Create(path, srgb);
#if defined(SNOW_PLATFORM_WINDOWS)
                case RenderAPIType::DirectX:    return Ref<DirectX11Texture2D>::Create(path, srgb);
#endif
                }
            }

            uint32_t Texture::GetBPP(TextureFormat format) {
                switch(format){
                case TextureFormat::RGB:    return 3;
                case TextureFormat::RGBA:    return 4;
                }
                return 0;
            }
        }
    }
}