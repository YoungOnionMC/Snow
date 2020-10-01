#include <spch.h>
#include "Snow/Render/API/Texture.h"

#include "Snow/Platform/OpenGL/OpenGLTexture.h"

#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Render {
        namespace API {
            Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap) {
                switch(Render::Renderer::GetRenderAPI()) {
                case RenderAPI::None:   return nullptr;
                case RenderAPI::OpenGL: return Ref<OpenGLTexture2D>::Create(format, width, height, wrap);   
                }
            }

            Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb) {
                switch(Render::Renderer::GetRenderAPI()) {
                case RenderAPI::None:   return nullptr;
                case RenderAPI::OpenGL: return Ref<OpenGLTexture2D>::Create(path, srgb);   
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