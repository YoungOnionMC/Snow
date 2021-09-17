#pragma once
#include "Snow/Render/Texture.h"

#include "Snow/Core/Buffer.h"

#include "Snow/Render/Image.h"

namespace Snow {
    using namespace Render;
    class OpenGLTexture2D : public Render::Texture2D {
    public:
        OpenGLTexture2D(Render::ImageFormat format, uint32_t width, uint32_t height, TextureWrap wrap);
        OpenGLTexture2D(const std::string& path, bool srgb);
        virtual ~OpenGLTexture2D();

        void Bind(uint32_t slot) const override;
        //void BindImageTexture(uint32_t slot, uint32_t level, bool access, Render::ImageFormat format) override {}

        void ResizeBuffer(uint32_t width, uint32_t height) override {}
        Buffer GetWriteableBuffer() override;

        virtual void CopyImageData(Ref<Texture>& otherTex) override {}
        //uint32_t GetMipLevelCount() const override {}


        void Lock() override;
        void Unlock() override;

        void SetData(void* data, uint32_t size) override;

        Render::ImageFormat GetFormat() const override {
            return {};
        }//m_Image->GetFormat(); }
        uint32_t GetWidth() const override { return m_Image->GetWidth(); }
        uint32_t GetHeight() const override { return m_Image->GetHeight(); }

        Ref<Render::Image2D> GetImage() const override { return m_Image; }

        uint64_t GetHash() const override { return 0; }

        void GenerateMips() override;

        const std::string& GetPath() const override { return m_Path; }

        //uint32_t GetRendererID() const override { return m_RendererID; }

    private:
        Ref<Image2D> m_Image;

        TextureWrap m_Wrap;

        std::string m_Path;

        bool m_IsHDR = false;

        bool m_Locked;

    };

    class OpenGLTextureCube : public TextureCube {
    public:
        OpenGLTextureCube(ImageFormat format, uint32_t width, uint32_t height);
        OpenGLTextureCube(const std::string& path, bool srgb);

        //uint32_t GetRendererID() const { return m_RendererID; }

        virtual ~OpenGLTextureCube();

        void Bind(uint32_t slot) const override;
        //void BindImageTexture(uint32_t slot, uint32_t level, bool access, API::TextureFormat format) override;

        virtual void CopyImageData(Ref<Texture>& otherTex) override;
        //uint32_t GetMipLevelCount() const override;

        void GenerateMips() override;

        ImageFormat GetFormat() const override { return m_Format; }
        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }

        const std::string& GetPath() const { return m_Path; }

        uint64_t GetHash() const override { return 0; }
    private:
        uint32_t m_RendererID;

        ImageFormat m_Format;
        TextureWrap m_Wrap;
        uint32_t m_Width = 0, m_Height = 0;

        std::string m_Path;

        Buffer m_ImageData;
    };
}