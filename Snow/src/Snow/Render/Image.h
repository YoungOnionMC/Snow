#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Core/Buffer.h"

#include <glm/glm.hpp>

namespace Snow {
	namespace Render {
		enum class ImageFormat {
			None,
			Red,
			RG,
			RGB,
			SRGB,
			RGBA,
			RGBA16F,
			RGBA32F,

			Depth32F,
			Depth24Stencil8,
			DepthStencil = Depth24Stencil8,
		};

		enum class ImageUsage {
			None = 0,
			Texture,
			Attachment,
			Storage
		};

		enum class TextureWrap {
			None = 0,
			Clamp,
			Repeat,

		};

		enum class TextureFilter {
			None = 0,
			Linear,
			Nearest
		};

		enum class TextureType {
			None = 0,
			Texture2D,
			TextureCube
		};

		struct TextureProperties {
			TextureWrap SamplerWrap = TextureWrap::Repeat;
			TextureFilter SamplerFilter = TextureFilter::Linear;
			bool GenerateMips = true;
			bool SRGB = false;
			bool Storage = false;
		};

		struct ImageSpecification {
			ImageFormat Format = ImageFormat::RGBA;
			ImageUsage Usage = ImageUsage::Texture;

			uint32_t Width = 1;
			uint32_t Height = 1;
			uint32_t Mips = 1;
			uint32_t Layers = 1;
			std::string DebugName = "";
			//bool Deinterleaved = false;
		};

		class Image : public RefCounted {
		public:
			virtual ~Image() {}

			virtual void Invalidate() = 0;
			virtual void Release() = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual ImageSpecification& GetSpecification() = 0;
			virtual const ImageSpecification& GetSpecification() const = 0;

			virtual Buffer GetBuffer() const = 0;
			virtual Buffer& GetBuffer() = 0;

			virtual void CreatePerLayerImageViews() = 0;

			virtual uint64_t GetHash() const = 0;
		};

		class Image2D : public Image {
		public:
			static Ref<Image2D> Create(ImageSpecification specification, Buffer buffer);
			static Ref<Image2D> Create(ImageSpecification specification, const void* data = nullptr);

			virtual void SetData(const void* data) = 0;
			virtual bool GetImageData() = 0;

			virtual void Resize(uint32_t width, uint32_t height) = 0;
		};

		namespace Utils {
			inline uint32_t GetFormatSize(ImageFormat format) {
				switch (format) {
				case ImageFormat::Red:	return 1;
				case ImageFormat::RG:	return 2;
				case ImageFormat::RGB:	
				case ImageFormat::SRGB:	return 3;
				case ImageFormat::RGBA:	return 4;
				case ImageFormat::RGBA16F:	return 2 * 4;
				case ImageFormat::RGBA32F:	return 4 * 4;
				}
			}

			inline uint32_t CalculateMipCount(uint32_t width, uint32_t height) {
				return (uint32_t)std::floor(std::log2(glm::min(width, height))) + 1;
			}

			inline uint32_t GetImageMemorySize(ImageFormat format, uint32_t width, uint32_t height) {
				return width * height * GetFormatSize(format);
			}

			inline bool IsDepthFormat(ImageFormat format) {
				if (format == ImageFormat::Depth24Stencil8 || format == ImageFormat::Depth32F)
					return true;
				return false;
			}
		}
	}
}