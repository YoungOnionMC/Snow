#pragma once
#include "Snow/Render/API/Texture.h"

#include "Snow/Core/Buffer.h"

namespace Snow {
    namespace Render {
        class OpenGLTexture2D : public API::Texture2D {
        public:
            OpenGLTexture2D(API::TextureFormat format, uint32_t width, uint32_t height, API::TextureWrap wrap);
            OpenGLTexture2D(const std::string& path, bool srgb);
            virtual ~OpenGLTexture2D();

            void Bind(uint32_t slot) const override;
            void ResizeBuffer(uint32_t width, uint32_t height) override;
            Buffer GetWriteableBuffer() override;

            void Lock() override;
            void Unlock() override;

            API::TextureFormat GetFormat() const override { return m_Format; }
            uint32_t GetWidth() const override { return m_Width; }
            uint32_t GetHeight() const override { return m_Height; }

            const std::string& GetPath() const override { return m_Path; }

            uint32_t GetRendererID() const { return m_RendererID; }

        private:
            uint32_t m_RendererID;
            API::TextureFormat m_Format;
            API::TextureWrap m_Wrap;
            uint32_t m_Width, m_Height;

            std::string m_Path;

            Buffer m_ImageData;

            bool m_Locked;

        };

        class OpenGLTextureCube : public API::TextureCube {

        };
    }
}