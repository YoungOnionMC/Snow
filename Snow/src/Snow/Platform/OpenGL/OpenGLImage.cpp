#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLImage.h"

#include <glad/glad.h>

namespace Snow {
	OpenGLImage2D::OpenGLImage2D(Render::ImageSpecification spec, uint32_t width, uint32_t height, Buffer buffer) :
		m_Specification(spec), m_Width(width), m_Height(height) {

		uint32_t size = Render::Utils::GetFormatSize(spec.Format);


		m_ImageData = Buffer::Copy(buffer.Data, size);
	}

	void OpenGLImage2D::Release() {
		glDeleteTextures(1, &m_ImageID);
		m_ImageID = 0;
		m_ImageData.Release();
	}

	void OpenGLImage2D::Invalidate() {
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ImageID);


		GLenum format = Utils::GetOpenGLImageFormat(m_Specification.Format);
		GLenum internalFormat = Utils::GetOpenGLImageInternalFormat(m_Specification.Format);
		GLenum dataType = Utils::GetOpenGLFormatDataType(m_Specification.Format);

		uint32_t mipCount = Render::Utils::CalculateMipCount(m_Width, m_Height);
		glTextureStorage2D(m_ImageID, mipCount, internalFormat, m_Width, m_Height);
		if (m_ImageData) {
			glTextureSubImage2D(m_ImageID, 0, 0, 0, m_Width, m_Height, format, dataType, m_ImageData.Data);
			glGenerateTextureMipmap(m_ImageID);
		}

		glCreateSamplers(1, &m_SamplerID);
		
		glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}