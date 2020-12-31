#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {
	namespace Render {

		namespace Utils {
			static VkSampleCountFlagBits GetVulkanSampleCount(uint32_t samples) {
				switch (samples) {
				case 1:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
				case 2:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT;
				case 4:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT;
				case 8:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT;
				case 16:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT;
				case 32:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT;
				case 64:
					return VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;

				}

				return VkSampleCountFlagBits::VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
			}

			static VkFormat GetVulkanFormat(FramebufferTextureFormat format) {
				switch (format) {
				case FramebufferTextureFormat::RGBA8:
					return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
				case FramebufferTextureFormat::RGBA16F:
					return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
				case FramebufferTextureFormat::RGBA32F:
					return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
				case FramebufferTextureFormat::RG32F:
					return VkFormat::VK_FORMAT_R32G32_SFLOAT;
				case FramebufferTextureFormat::Depth32F:
					return VkFormat::VK_FORMAT_D32_SFLOAT;
				case FramebufferTextureFormat::Depth32Stencil8:
					return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
				}

				return VkFormat::VK_FORMAT_UNDEFINED;
			}

			static void CreateColorAttachment(ImageAttachment& imageAttachment, uint32_t index, uint32_t samples, FramebufferTextureFormat format, uint32_t width, uint32_t height, VulkanAllocator& allocator, VkAttachmentDescription& attachmentDesc, VkAttachmentReference& attachmentRef) {
				auto vkDevice = VulkanContext::GetCurrentDevice();

				
				const VkFormat imageFormat = GetVulkanFormat(format);

				VkImageCreateInfo imageCI;
				ZeroInitVulkanStruct(imageCI, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
				imageCI.imageType = VK_IMAGE_TYPE_2D;
				imageCI.format = imageFormat;
				imageCI.extent.width = width;
				imageCI.extent.height = height;
				imageCI.extent.depth = 1;
				imageCI.mipLevels = 1;
				imageCI.arrayLayers = 1;
				imageCI.samples = GetVulkanSampleCount(samples);
				imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageCI.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

				VKCheckError(vkCreateImage(vkDevice->GetVulkanDevice(), &imageCI, nullptr, &(imageAttachment.Image)));

				VkMemoryRequirements memReqs;
				vkGetImageMemoryRequirements(vkDevice->GetVulkanDevice(), imageAttachment.Image, &memReqs);

				allocator.Allocate(memReqs, &imageAttachment.Memory);

				VKCheckError(vkBindImageMemory(vkDevice->GetVulkanDevice(), imageAttachment.Image, imageAttachment.Memory, 0));

				VkImageViewCreateInfo imageViewCI;
				ZeroInitVulkanStruct(imageViewCI, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
				imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewCI.format = imageFormat;
				imageViewCI.image = imageAttachment.Image;
				imageViewCI.subresourceRange = {};
				imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewCI.subresourceRange.baseMipLevel = 0;
				imageViewCI.subresourceRange.levelCount = 1;
				imageViewCI.subresourceRange.baseArrayLayer = 0;
				imageViewCI.subresourceRange.layerCount = 1;

				VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &imageViewCI, nullptr, &imageAttachment.ImageView));

				attachmentDesc.flags = 0;
				attachmentDesc.format = imageFormat;
				attachmentDesc.samples = GetVulkanSampleCount(samples);
				attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

				attachmentRef = { index, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
			}

			static void CreateDepthAttachment(ImageAttachment& imageAttachment, uint32_t samples, FramebufferTextureFormat format, uint32_t width, uint32_t height, VulkanAllocator& allocator, VkAttachmentDescription& attachmentDesc) {
				auto vkDevice = VulkanContext::GetCurrentDevice();


				const VkFormat imageFormat = GetVulkanFormat(format);

				VkImageCreateInfo imageCI = {};
				ZeroInitVulkanStruct(imageCI, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);
				imageCI.imageType = VK_IMAGE_TYPE_2D;
				imageCI.format = imageFormat;
				imageCI.extent.width = width;
				imageCI.extent.height = height;
				imageCI.extent.depth = 1;
				imageCI.mipLevels = 1;
				imageCI.arrayLayers = 1;
				imageCI.samples = GetVulkanSampleCount(samples);
				imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

				VKCheckError(vkCreateImage(vkDevice->GetVulkanDevice(), &imageCI, nullptr, &imageAttachment.Image));

				VkMemoryRequirements memReqs;
				vkGetImageMemoryRequirements(vkDevice->GetVulkanDevice(), imageAttachment.Image, &memReqs);

				allocator.Allocate(memReqs, &imageAttachment.Memory);

				VKCheckError(vkBindImageMemory(vkDevice->GetVulkanDevice(), imageAttachment.Image, imageAttachment.Memory, 0));

				VkImageViewCreateInfo imageViewCI;
				ZeroInitVulkanStruct(imageViewCI, VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO);
				imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewCI.format = imageFormat;
				imageViewCI.image = imageAttachment.Image;
				imageViewCI.subresourceRange = {};
				imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				if (format == FramebufferTextureFormat::Depth32Stencil8)
					imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
				imageViewCI.subresourceRange.baseMipLevel = 0;
				imageViewCI.subresourceRange.levelCount = 1;
				imageViewCI.subresourceRange.baseArrayLayer = 0;
				imageViewCI.subresourceRange.layerCount = 1;

				VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &imageViewCI, nullptr, &imageAttachment.ImageView));

				attachmentDesc.flags = 0;
				attachmentDesc.format = imageFormat;
				attachmentDesc.samples = GetVulkanSampleCount(samples);
				attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			}

			static bool IsDepthFormat(Render::FramebufferTextureFormat format) {
				switch (format) {
				case Render::FramebufferTextureFormat::Depth32Stencil8:
				case Render::FramebufferTextureFormat::Depth32F:
					return true;
				}
				return false;
			}
		}

		VulkanFramebuffer::VulkanFramebuffer(const FramebufferSpecification& spec) :
			m_Specification(spec) {

			for (auto format : m_Specification.AttachmentList.Attachments) {
				if (!Utils::IsDepthFormat(format.TextureFormat)) 
					m_ColorAttachmentFormats.emplace_back(format.TextureFormat);
				else
					m_DepthAttachmentFormat = format.TextureFormat;
			}

			Resize(spec.Width, spec.Height);
		}

		VulkanFramebuffer::~VulkanFramebuffer() {

		}

		void VulkanFramebuffer::Resize(uint32_t width, uint32_t height) {
			m_Width = width;
			m_Height = height;

			if (!m_Specification.SwapChainTarget) {
				auto vkDevice = VulkanContext::GetCurrentDevice();

				if (m_VulkanFramebufferHandle) {
					vkDestroyFramebuffer(vkDevice->GetVulkanDevice(), m_VulkanFramebufferHandle, nullptr);
					
				}

				VulkanAllocator allocator("Framebuffer");

				if (m_ColorAttachmentFormats.size()) {
					m_ColorAttachments.resize(m_ColorAttachmentFormats.size());
					m_ColorAttachmentDescriptions.resize(m_ColorAttachmentFormats.size());
					m_ColorAttachmentReferences.resize(m_ColorAttachmentFormats.size());
					
					for (uint32_t i = 0; i < m_ColorAttachments.size(); i++) {
						m_ColorAttachments[i] = ImageAttachment();
						Utils::CreateColorAttachment(m_ColorAttachments[i], i, m_Specification.Samples, m_ColorAttachmentFormats[i], width, height, allocator, m_ColorAttachmentDescriptions[i], m_ColorAttachmentReferences[i]);
						m_ColorAttachments[i].ClearColor = m_Specification.ClearColor;
					}

				}

				if (m_DepthAttachmentFormat != FramebufferTextureFormat::None) {
					Utils::CreateDepthAttachment(m_DepthAttachment, m_Specification.Samples, m_DepthAttachmentFormat, width, height, allocator, m_DepthAttachmentDescription);
					m_DepthAttachmentReference = { (uint32_t)m_ColorAttachmentReferences.size(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
				}

				

				VkSubpassDescription subpassDesc = {};
				subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				subpassDesc.colorAttachmentCount = m_ColorAttachments.size();
				subpassDesc.pColorAttachments = m_ColorAttachmentReferences.data();
				if(m_DepthAttachmentFormat!=FramebufferTextureFormat::None)
					subpassDesc.pDepthStencilAttachment = &m_DepthAttachmentReference;

				std::array<VkSubpassDependency, 2> dependencies;

				dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[0].dstSubpass = 0;
				dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				dependencies[1].srcSubpass = 0;
				dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
				dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				std::vector<VkAttachmentDescription> descriptions = m_ColorAttachmentDescriptions;
				if(m_DepthAttachmentFormat != FramebufferTextureFormat::None)
					descriptions.push_back(m_DepthAttachmentDescription);

				VkRenderPassCreateInfo renderpassCI;
				ZeroInitVulkanStruct(renderpassCI, VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO);
				renderpassCI.attachmentCount = descriptions.size();
				renderpassCI.pAttachments = descriptions.data();
				renderpassCI.subpassCount = 1;
				renderpassCI.pSubpasses = &subpassDesc;
				renderpassCI.dependencyCount = dependencies.size();
				renderpassCI.pDependencies = dependencies.data();
				
				VKCheckError(vkCreateRenderPass(vkDevice->GetVulkanDevice(), &renderpassCI, nullptr, &m_RenderPass));

				std::vector<VkImageView> attachments;
				attachments.resize(m_ColorAttachments.size());
				for (uint32_t i = 0; i < m_ColorAttachments.size(); i++)
					attachments[i] = m_ColorAttachments[i].ImageView;

				if (m_DepthAttachmentFormat != FramebufferTextureFormat::None)
					attachments.push_back(m_DepthAttachment.ImageView);

				VkFramebufferCreateInfo framebufferCI;
				ZeroInitVulkanStruct(framebufferCI, VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO);
				framebufferCI.renderPass = m_RenderPass;
				framebufferCI.attachmentCount = attachments.size();
				framebufferCI.pAttachments = attachments.data();
				framebufferCI.width = width;
				framebufferCI.height = height;
				framebufferCI.layers = 1;

				VKCheckError(vkCreateFramebuffer(vkDevice->GetVulkanDevice(), &framebufferCI, nullptr, &m_VulkanFramebufferHandle));
			}
			else {
				VulkanSwapChain& vkSwapChain = VulkanContext::Get()->GetSwapChain();
				m_RenderPass = vkSwapChain.GetRenderPass();
			}
		}
	}
}