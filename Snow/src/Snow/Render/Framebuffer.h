#pragma once

#include <glm/glm.hpp>

#include "Snow/Core/Ref.h"

#include "Snow/Render/Image.h"

namespace Snow {
	namespace Render {

		class Framebuffer;

		enum class FramebufferBlendMode {
			None = 0,
			OneZero,
			SrcAlphaOneMinusSrcAlpha,
			Additive,
			ZeroSrcColor
		};

		struct FramebufferTextureSpecification {
			FramebufferTextureSpecification() = default;
			FramebufferTextureSpecification(ImageFormat format) : Format(format) {}

			ImageFormat Format;
			bool Blend = true;
			FramebufferBlendMode BlendMode = FramebufferBlendMode::SrcAlphaOneMinusSrcAlpha;
		};

		struct FramebufferAttachmentSpecification {
			FramebufferAttachmentSpecification() = default;
			FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification>& attachments) :
				Attachments(attachments) {}

			std::vector<FramebufferTextureSpecification> Attachments;
		};

		struct FramebufferSpecification {
			float Scale = 1.0f;
			uint32_t Width = 0, Height = 0;
			glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			float ClearDepthValue = 0.0f;
			bool ClearColorOnLoad = true;
			bool ClearDepthOnLoad = true;
			FramebufferAttachmentSpecification AttachmentList;
			uint32_t Samples = 1;

			bool Transfer = false;

			bool NoResize = false;

			bool Blend = true;
			FramebufferBlendMode BlendMode = FramebufferBlendMode::None;

			bool SwapChainTarget = false;

			Ref<Image2D> ExistingImage;
			std::vector<uint32_t> ExistingImageLayers;

			std::map<uint32_t, Ref<Image2D>> ExistingImages;

			Ref<Framebuffer> ExistingFramebuffer;

			std::string DebugName;
		};

		class Framebuffer : public RefCounted {
		public:
			virtual ~Framebuffer() {}

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual Ref<Image2D> GetImage(uint32_t attachmentIndex = 0) const = 0;
			virtual Ref<Image2D> GetDepthImage() const = 0;

			virtual const FramebufferSpecification& GetSpecification() const = 0;

			static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

		};
	}
}