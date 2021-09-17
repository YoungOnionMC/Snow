#pragma once

#include "Snow/Render/Image.h"

#include "Snow/Core/Buffer.h"

#include <glad/glad.h>

namespace Snow {
	class OpenGLImage2D : public Render::Image2D {
	public:
		
		OpenGLImage2D(Render::ImageSpecification spec, uint32_t width, uint32_t height, Buffer buffer);

		void Invalidate() override;
		void Release() override;

		Render::ImageFormat GetFormat() const override { return m_Specification.Format; }
		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }

		Buffer& GetBuffer() override { return m_ImageData; }
		Buffer GetBuffer() const override { return m_ImageData; }

		virtual uint64_t GetHash() const override { return 0; }

	private:
		uint32_t m_ImageID;
		uint32_t m_SamplerID;

		uint32_t m_Width;
		uint32_t m_Height;

		Buffer m_ImageData;

		Render::ImageSpecification m_Specification;
	};

	namespace Utils {
		using namespace Render;
		inline GLenum GetOpenGLImageFormat(ImageFormat format) {
			switch (format) {
			case ImageFormat::Red:	return GL_RED;
			case ImageFormat::RG:	return GL_RG;
			case ImageFormat::RGB:	return GL_RGB;
			case ImageFormat::SRGB:	return GL_RGB;
			case ImageFormat::RGBA:	return GL_RGBA;
			case ImageFormat::Float16:	return GL_RGBA16F;
			case ImageFormat::Float32:	return GL_RGBA32F;
			case ImageFormat::Depth32F:	return GL_DEPTH32F_STENCIL8;
			case ImageFormat::Depth24Stencil8:	return GL_DEPTH24_STENCIL8;
			}
		}

		inline GLenum GetOpenGLImageInternalFormat(ImageFormat format) {
			switch (format) {
			case ImageFormat::Red:	return GL_R8;
			case ImageFormat::RG:	return GL_RG8;
			case ImageFormat::RGB:	return GL_RGB8;
			case ImageFormat::SRGB:	return GL_SRGB8;
			case ImageFormat::RGBA:	return GL_RGBA8;
			case ImageFormat::Float16:	return GL_RGBA16F;
			case ImageFormat::Float32:	return GL_RGBA32F;

			}
		}

		inline GLenum GetOpenGLFormatDataType(ImageFormat format) {
			switch (format) {
			case ImageFormat::Red:
			case ImageFormat::RG:
			case ImageFormat::RGB:
			case ImageFormat::SRGB:	
			case ImageFormat::RGBA:	return GL_UNSIGNED_BYTE;
			case ImageFormat::Float16:
			case ImageFormat::Float32:	return GL_FLOAT;
			}
		}
	}
}