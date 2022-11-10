#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Platform/Vulkan/VulkanImage.h"

namespace Snow {

	VulkanFramebuffer::VulkanFramebuffer(const Render::FramebufferSpecification& spec) :
		m_Specification(spec) {

		if (spec.Width == 0) {
			m_Width = Core::Application::Get().GetWindow()->GetWidth();
			m_Height = Core::Application::Get().GetWindow()->GetHeight();
		}
		else {
			m_Width = spec.Width;
			m_Height = spec.Height;
		}


		uint32_t attachmentIndex = 0;
		if (!m_Specification.ExistingFramebuffer) {
			for (auto& attachmentSpec : m_Specification.AttachmentList.Attachments) {
				if (m_Specification.ExistingImage && m_Specification.ExistingImage->GetSpecification().Layers > 1) {
					//SNOW_CORE_ASSERT(!Render::Utils::IsDepthFormat(attachmentSpec.Format), "Only supported for color attachments");
					if (Render::Utils::IsDepthFormat(attachmentSpec.Format))
						m_DepthAttachmentImage = m_Specification.ExistingImage;
					else
						m_ColorAttachmentImages.emplace_back(m_Specification.ExistingImage);
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end()) {
					if (!Render::Utils::IsDepthFormat(attachmentSpec.Format))
						m_ColorAttachmentImages.emplace_back();
				}
				else if (Render::Utils::IsDepthFormat(attachmentSpec.Format)) {
					Render::ImageSpecification imageSpec;
					imageSpec.Format = attachmentSpec.Format;
					imageSpec.Usage = Render::ImageUsage::Attachment;
					imageSpec.Width = m_Width;
					imageSpec.Height = m_Height;
					imageSpec.DebugName = fmt::format("{0}-DepthAttachment{1}", m_Specification.DebugName.empty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_DepthAttachmentImage = Render::Image2D::Create(imageSpec);
				}
				else {
					Render::ImageSpecification imageSpec;
					imageSpec.Format = attachmentSpec.Format;
					imageSpec.Usage = Render::ImageUsage::Attachment;
					imageSpec.Width = m_Width;
					imageSpec.Height = m_Height;
					imageSpec.DebugName = fmt::format("{0}-ColorAttachment{1}", m_Specification.DebugName.empty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
					m_ColorAttachmentImages.emplace_back(Render::Image2D::Create(imageSpec));
				}
				attachmentIndex++;
			}
		}
		SNOW_CORE_ASSERT(spec.AttachmentList.Attachments.size())
			Resize(m_Width, m_Height, true);
	}

	VulkanFramebuffer::~VulkanFramebuffer() {

	}

	void VulkanFramebuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate) {
		if (!forceRecreate && (m_Width == width && m_Height == height))
			return;

		m_Width = width;
		m_Height = height;

		if (!m_Specification.SwapChainTarget) {
			Invalidate();
		}
		else {
			Ref<VulkanSwapChain> vkSwapChain = Core::Application::Get().GetWindow()->GetSwapChain().As<VulkanSwapChain>();
			m_RenderPass = vkSwapChain->GetRenderPass();

			m_ClearValues.clear();
			m_ClearValues.emplace_back().color = { 0.0f, 0.0f, 0.0f, 1.0f };
		}
	}

	void VulkanFramebuffer::Invalidate() {
		Ref<VulkanFramebuffer> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->RTInvalidate();
			});
	}

	void VulkanFramebuffer::RTInvalidate() {
		auto vkDevice = VulkanContext::GetCurrentDevice();

		if (m_Framebuffer) {
			VkFramebuffer framebuffer = m_Framebuffer;
			Render::Renderer::SubmitResourceFree([framebuffer]() {
				auto device = VulkanContext::GetCurrentDevice();
				vkDestroyFramebuffer(device->GetVulkanDevice(), framebuffer, nullptr);
				});

			if (!m_Specification.ExistingFramebuffer) {
				uint32_t attachmentIndex = 0;
				for (Ref<VulkanImage2D> image : m_ColorAttachmentImages) {
					if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
						continue;
					if (image->GetSpecification().Layers == 1 || attachmentIndex == 0 && !image->GetLayerImageView(0))
						image->Release();
					attachmentIndex++;
				}
				if (m_DepthAttachmentImage) {
					if (m_Specification.ExistingImages.find((uint32_t)m_Specification.AttachmentList.Attachments.size() - 1) == m_Specification.ExistingImages.end())
						m_DepthAttachmentImage->Release();
				}
			}
		}

		VulkanAllocator allocator("Framebuffer");

		std::vector<VkAttachmentDescription> attachmentDescriptions;

		std::vector<VkAttachmentReference> colorAttachmentRefs;
		VkAttachmentReference depthAttachmentRef;

		m_ClearValues.resize(m_Specification.AttachmentList.Attachments.size());

		bool createImages = m_ColorAttachmentImages.empty();

		if (m_Specification.ExistingFramebuffer)
			m_ColorAttachmentImages.clear();

		uint32_t attachmentIndex = 0;
		for (auto attachmentSpec : m_Specification.AttachmentList.Attachments) {
			if (Render::Utils::IsDepthFormat(attachmentSpec.Format)) {
				if (m_Specification.ExistingImage) {
					m_DepthAttachmentImage = m_Specification.ExistingImage;
				}
				else if (m_Specification.ExistingFramebuffer) {
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					m_DepthAttachmentImage = existingFramebuffer->GetDepthImage();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end()) {
					Ref<Render::Image2D> existingImage = m_Specification.ExistingImages.at(attachmentIndex);
					SNOW_CORE_ASSERT(Render::Utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach non-depth image as depth attachment");
					m_DepthAttachmentImage = existingImage;
				}
				else {
					Ref<VulkanImage2D> depthAttachmentImage = m_DepthAttachmentImage.As<VulkanImage2D>();
					auto& spec = depthAttachmentImage->GetSpecification();
					spec.Width = m_Width;
					spec.Height = m_Height;
					depthAttachmentImage->RTInvalidate();
				}

				VkAttachmentDescription& attachmentDesc = attachmentDescriptions.emplace_back();
				attachmentDesc.flags = 0;
				attachmentDesc.format = Render::Utils::GetVulkanFormat(attachmentSpec.Format);
				attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDesc.loadOp = m_Specification.ClearDepthOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

				attachmentDesc.initialLayout = m_Specification.ClearDepthOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
				if (attachmentSpec.Format == Render::ImageFormat::Depth24Stencil8 || true) {
					// TODO: check if sampling
					attachmentDesc.finalLayout = true ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					depthAttachmentRef = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL };
				}
				else {
					// TODO: check if sampling
					attachmentDesc.finalLayout = true ? VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
					depthAttachmentRef = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL };
				}
				m_ClearValues[attachmentIndex].depthStencil = { m_Specification.ClearDepthValue, 0 };
			}
			else {
				Ref<VulkanImage2D> colorAttachment;
				if (m_Specification.ExistingFramebuffer) {
					Ref<VulkanFramebuffer> existingFramebuffer = m_Specification.ExistingFramebuffer.As<VulkanFramebuffer>();
					Ref<Render::Image2D> existingImage = existingFramebuffer->GetImage(attachmentIndex);
					colorAttachment = m_ColorAttachmentImages.emplace_back(existingImage).As<VulkanImage2D>();
				}
				else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end()) {
					Ref<Render::Image2D> existingImage = m_Specification.ExistingImages[attachmentIndex];
					SNOW_CORE_ASSERT(!Render::Utils::IsDepthFormat(existingImage->GetSpecification().Format), "Trying to attach depth image as color attachment");
					colorAttachment = existingImage.As<VulkanImage2D>();
					m_ColorAttachmentImages[attachmentIndex] = colorAttachment;
				}
				else {
					if (createImages) {
						Render::ImageSpecification imageSpec;
						imageSpec.Format = attachmentSpec.Format;
						imageSpec.Usage = Render::ImageUsage::Attachment;
						imageSpec.Width = m_Width;
						imageSpec.Height = m_Height;
						colorAttachment = m_ColorAttachmentImages.emplace_back(Render::Image2D::Create(imageSpec)).As<VulkanImage2D>();
					}
					else {
						Ref<Render::Image2D> colorAttachmentImage = m_ColorAttachmentImages[attachmentIndex];
						auto& spec = colorAttachmentImage->GetSpecification();
						spec.Width = m_Width;
						spec.Height = m_Height;
						colorAttachment = colorAttachmentImage.As<VulkanImage2D>();
						if (colorAttachment->GetSpecification().Layers == 1)
							colorAttachment->RTInvalidate();
						else if (attachmentIndex == 0 && m_Specification.ExistingImageLayers[0] == 0) {
							colorAttachment->RTInvalidate();
							colorAttachment->RTCreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
						else if (attachmentIndex == 0) {
							colorAttachment->RTCreatePerSpecificLayerImageViews(m_Specification.ExistingImageLayers);
						}
					}
				}

				VkAttachmentDescription& attachmentDesc = attachmentDescriptions.emplace_back();
				attachmentDesc.flags = 0;
				attachmentDesc.format = Render::Utils::GetVulkanFormat(attachmentSpec.Format);
				attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDesc.loadOp = m_Specification.ClearColorOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
				attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDesc.initialLayout = m_Specification.ClearColorOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				const auto& clearColor = m_Specification.ClearColor;
				m_ClearValues[attachmentIndex].color = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a} };
				colorAttachmentRefs.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
			}
			attachmentIndex++;
		}

		VkSubpassDescription subpassDesc = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = uint32_t(colorAttachmentRefs.size());
		subpassDesc.pColorAttachments = colorAttachmentRefs.data();
		if (m_DepthAttachmentImage)
			subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;


		std::vector<VkSubpassDependency> dependencies;
		if (m_ColorAttachmentImages.size()) {
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = 0;
				dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}

		}

		if (m_DepthAttachmentImage) {
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
				dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
			{
				VkSubpassDependency& dependency = dependencies.emplace_back();
				dependency.srcSubpass = 0;
				dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				dependency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				dependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			}
		}

		VkRenderPassCreateInfo renderPassCI = {};
		renderPassCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCI.attachmentCount = attachmentDescriptions.size();
		renderPassCI.pAttachments = attachmentDescriptions.data();
		renderPassCI.subpassCount = 1;
		renderPassCI.pSubpasses = &subpassDesc;
		renderPassCI.dependencyCount = dependencies.size();
		renderPassCI.pDependencies = dependencies.data();

		VKCheckError(vkCreateRenderPass(vkDevice->GetVulkanDevice(), &renderPassCI, nullptr, &m_RenderPass));

		std::vector<VkImageView> attachments(m_ColorAttachmentImages.size());
		for (uint32_t i = 0; i < m_ColorAttachmentImages.size(); i++) {
			Ref<VulkanImage2D> image = m_ColorAttachmentImages[i].As<VulkanImage2D>();
			if (image->GetSpecification().Layers > 1) {
				attachments[i] = image->GetLayerImageView(m_Specification.ExistingImageLayers[i]);
				SNOW_CORE_ASSERT(attachments[i]);
			}
			else {
				attachments[i] = image->GetImageInfo().ImageView;
				SNOW_CORE_ASSERT(attachments[i]);
			}
		}

		if (m_DepthAttachmentImage) {
			Ref<VulkanImage2D> image = m_DepthAttachmentImage.As<VulkanImage2D>();
			if (m_Specification.ExistingImage) {
				SNOW_CORE_ASSERT(m_Specification.ExistingImageLayers.size() == 1, "Depth attachments do not support deinterleaving");
				attachments.emplace_back(image->GetLayerImageView(m_Specification.ExistingImageLayers[0]));
				SNOW_CORE_ASSERT(attachments.back());
			}
			else {
				attachments.emplace_back(image->GetImageInfo().ImageView);
				SNOW_CORE_ASSERT(attachments.back());
			}
		}

		VkFramebufferCreateInfo framebufferCI = {};
		framebufferCI.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCI.renderPass = m_RenderPass;
		framebufferCI.attachmentCount = uint32_t(attachments.size());
		framebufferCI.pAttachments = attachments.data();
		framebufferCI.width = m_Width;
		framebufferCI.height = m_Height;
		framebufferCI.layers = 1;

		VKCheckError(vkCreateFramebuffer(vkDevice->GetVulkanDevice(), &framebufferCI, nullptr, &m_Framebuffer));

	}
}