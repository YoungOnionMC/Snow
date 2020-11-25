#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

#include <stb_image.h>

namespace Snow {
    namespace Render {

        GLenum SnowToOpenGLFormat(API::TextureFormat format) {
            switch(format) {
            case API::TextureFormat::RGB:   return GL_RGB;
            case API::TextureFormat::RGBA:   return GL_RGBA;
            case API::TextureFormat::Float16:   return GL_RGBA16F;
            }
            return 0;
        }

        GLenum SnowToOpenGLWrap(API::TextureWrap wrap) {
            switch(wrap) {
            case API::TextureWrap::Repeat:   return GL_REPEAT;
            case API::TextureWrap::Clamp:   return GL_CLAMP;
            }
            return 0;
        }

        OpenGLTexture2D::OpenGLTexture2D(API::TextureFormat format, uint32_t width, uint32_t height, API::TextureWrap wrap) :
            m_Format(format), m_Width(width), m_Height(height), m_Wrap(wrap), m_Locked(false) {

            

            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, SnowToOpenGLWrap(m_Wrap));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, SnowToOpenGLWrap(m_Wrap));

            glTexImage2D(GL_TEXTURE_2D, 0, SnowToOpenGLFormat(m_Format), m_Width, m_Height, 0, SnowToOpenGLFormat(m_Format), GL_UNSIGNED_BYTE, nullptr);

            glBindTexture(GL_TEXTURE_2D, 0);

            m_ImageData.Allocate(width * height * API::Texture::GetBPP(m_Format));
        }

        OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool srgb) :
            m_Path(path), m_Locked(false) {
            int width, height, channels;
            m_ImageData.Data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);
            m_Format = API::TextureFormat::RGBA;

            if(!m_ImageData.Data)
                return;

            m_Width = width;
            m_Height = height;

            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            GLenum internalFormat = SnowToOpenGLFormat(m_Format);
            GLenum format = srgb ? GL_SRGB8 : (SnowToOpenGLFormat(m_Format));
            GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, m_ImageData.Data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(m_ImageData.Data);
        }

        OpenGLTexture2D::~OpenGLTexture2D() {
            glDeleteTextures(1, &m_RendererID);
        }

        void OpenGLTexture2D::Bind(uint32_t slot) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
        }

        void OpenGLTexture2D::ResizeBuffer(uint32_t width, uint32_t height) {
            SNOW_CORE_ASSERT(m_Locked, "Trying to write to texture without it being locked");
            m_ImageData.Allocate(width * height * API::Texture::GetBPP(m_Format));
            m_ImageData.ZeroInitialize();
        }

        Buffer OpenGLTexture2D::GetWriteableBuffer() {
            SNOW_CORE_ASSERT(m_Locked, "Texture is currently uploaded to gpu");
            return m_ImageData;
        }

        void OpenGLTexture2D::Lock() {
            m_Locked = true;
        }

        void OpenGLTexture2D::Unlock() {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, SnowToOpenGLFormat(m_Format), GL_UNSIGNED_BYTE, m_ImageData.Data);
            m_Locked = false;
        }

        void OpenGLTexture2D::SetData(void* data, uint32_t size) {
            uint32_t bbp = SnowToOpenGLFormat(m_Format) == GL_RGBA ? 4 : 3;
            if (!(size == m_Width * m_Height * bbp)) SNOW_CORE_ERROR("Data must be entire texture");
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, SnowToOpenGLFormat(m_Format), GL_UNSIGNED_BYTE, data);
        }
    }
}