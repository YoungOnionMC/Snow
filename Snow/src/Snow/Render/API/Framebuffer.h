#pragma once

#include <glm/glm.hpp>

#include "Snow/Core/Ref.h"

namespace Snow {
	namespace Render {

		enum class FramebufferTextureFormat {
			None = 0,

			RGBA8 = 1,
			RGBA16F = 2,
			RGBA32F = 3,
			RG32F = 4,

			Depth32F = 5,
			Depth32Stencil8 = 6,

			Depth = Depth32Stencil8
		};

		struct FramebufferTextureSpecification {
			FramebufferTextureSpecification() = default;
			FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

			FramebufferTextureFormat TextureFormat;
		};

		struct FramebufferAttachmentSpecification {
			FramebufferAttachmentSpecification() = default;
			FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments) :
				Attachments(attachments) {}

			std::vector<FramebufferTextureSpecification> Attachments;
		};

		struct FramebufferSpecification {
			uint32_t Width = 0, Height = 0;
			glm::vec4 ClearColor;
			FramebufferAttachmentSpecification Attachments;
			uint32_t Samples = 1;

			bool SwapChainTarget = false;
		};

		class Framebuffer : public RefCounted {
		public:
			virtual ~Framebuffer() = default;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual void Resize(uint32_t width, uint32_t height) = 0;

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual uint32_t GetColorAttachmentRendererID(int index = 0) const = 0;
			virtual uint32_t GetDepthAttachmentRendererID() const = 0;

			virtual const FramebufferSpecification& GetSpecification() const = 0;

			static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		};
	}
}