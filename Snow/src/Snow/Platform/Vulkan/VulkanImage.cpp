#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanImage.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanRenderer.h"



namespace Snow {

	static std::map<VkImage, WeakRef<VulkanImage2D>> s_ImageReferences;

	//using namespace Render;
	VulkanImage2D::VulkanImage2D(Render::ImageSpecification spec) :
		m_Specification(spec) {}

	VulkanImage2D::~VulkanImage2D() {
		if (m_ImageInfo.Image) {

		}
	}

	void VulkanImage2D::Invalidate() {
		Ref<VulkanImage2D> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->RTInvalidate();
		});
		
	}

	void VulkanImage2D::Release() {
		if (m_ImageInfo.Image == nullptr)
			return;

		Ref<VulkanImage2D> instance = this;
		VulkanImageInfo info = m_ImageInfo;

		Render::Renderer::SubmitResourceFree([info, layerViews = m_PerLayerImageViews]() mutable {
			auto vkDevice = VulkanContext::GetCurrentDevice();
			SNOW_CORE_WARN("VulkanImage2D::Release ImageView = {0}", (const void*)info.ImageView);
			vkDestroyImageView(vkDevice->GetVulkanDevice(), info.ImageView, nullptr);
			vkDestroySampler(vkDevice->GetVulkanDevice(), info.Sampler, nullptr);

			for (auto& view : layerViews) {
				if (view)
					vkDestroyImageView(vkDevice->GetVulkanDevice(), view, nullptr);
			}

			VulkanAllocator allocator("VulkanImage2D");
			allocator.DestroyImage(info.Image, info.MemoryAllocation);
			s_ImageReferences.erase(info.Image);
		});
		m_ImageInfo.Image = nullptr;
		m_ImageInfo.ImageView = nullptr;
		m_ImageInfo.Sampler = nullptr;
		m_PerLayerImageViews.clear();

		
	}

	void VulkanImage2D::RTInvalidate() {
		
		Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();
		VulkanAllocator allocator("Image2D");

		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_Specification.Usage == Render::ImageUsage::Attachment) {
			if (Render::Utils::IsDepthFormat(m_Specification.Format))
				usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			else
				usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		else if (m_Specification.Usage == Render::ImageUsage::Texture) {
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}
		else if (m_Specification.Usage == Render::ImageUsage::Storage) {
			usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VkImageAspectFlags aspectMask = Render::Utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == Render::ImageFormat::Depth24Stencil8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkFormat vulkanFormat = Utils::GetVulkanFormat(m_Specification.Format);

		VkImageCreateInfo imageCI = {};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = vulkanFormat;
		imageCI.extent.width = m_Specification.Width;
		imageCI.extent.height = m_Specification.Height;
		imageCI.extent.depth = 1;
		imageCI.mipLevels = m_Specification.Mips;
		imageCI.arrayLayers = m_Specification.Layers;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = usage;
		m_ImageInfo.MemoryAllocation = allocator.AllocateImage(imageCI, VMA_MEMORY_USAGE_GPU_ONLY, m_ImageInfo.Image);
		s_ImageReferences[m_ImageInfo.Image] = this;

		// Create a default image view
		VkImageViewCreateInfo imageViewCI = {};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.viewType = m_Specification.Layers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.format = vulkanFormat;
		imageViewCI.flags = 0;
		imageViewCI.subresourceRange = {};
		imageViewCI.subresourceRange.aspectMask = aspectMask;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = m_Specification.Mips;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = m_Specification.Layers;
		imageViewCI.image = m_ImageInfo.Image;
		VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &imageViewCI, nullptr, &m_ImageInfo.ImageView));

		VkSamplerCreateInfo samplerCI = {};
		samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCI.maxAnisotropy = 1.0f;
		samplerCI.anisotropyEnable = VK_FALSE;
		samplerCI.magFilter = VK_FILTER_LINEAR;
		samplerCI.minFilter = VK_FILTER_LINEAR;
		samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCI.addressModeV = samplerCI.addressModeU;
		samplerCI.addressModeW = samplerCI.addressModeU;
		samplerCI.mipLodBias = 0.0f;
		samplerCI.compareOp = VK_COMPARE_OP_NEVER;
		samplerCI.minLod = 0.0f;
		samplerCI.maxLod = 1.0f;
		samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VKCheckError(vkCreateSampler(vkDevice->GetVulkanDevice(), &samplerCI, nullptr, &m_ImageInfo.Sampler));

		if (m_Specification.Usage == Render::ImageUsage::Storage) {
			VkCommandBuffer cmdBuffer = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = m_Specification.Mips;
			subresourceRange.layerCount = m_Specification.Layers;

			 Render::Utils::InsertImageMemoryBarrier(cmdBuffer, m_ImageInfo.Image, 
				0, 0, 
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, subresourceRange);

			VulkanContext::GetCurrentDevice()->FlushCommandBuffer(cmdBuffer);
		}

		UpdateDescriptor();
	}

	void VulkanImage2D::CreatePerLayerImageViews() {
		Ref<VulkanImage2D> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->RTCreatePerLayerImageViews();
		});
	}

	void VulkanImage2D::RTCreatePerLayerImageViews() {
		SNOW_CORE_ASSERT(m_Specification.Layers > 1);

		auto vkDevice = VulkanContext::GetCurrentDevice();

		VkImageAspectFlags aspectMask = Render::Utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == Render::ImageFormat::Depth24Stencil8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		const VkFormat vulkanFormat = Utils::GetVulkanFormat(m_Specification.Format);

		m_PerLayerImageViews.resize(m_Specification.Layers);
		for (uint32_t layer = 0; layer < m_Specification.Layers; layer++) {
			VkImageViewCreateInfo imageViewCI = {};
			imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCI.format = vulkanFormat;
			imageViewCI.flags = 0;
			imageViewCI.subresourceRange = {};
			imageViewCI.subresourceRange.aspectMask = aspectMask;
			imageViewCI.subresourceRange.baseMipLevel = 0;
			imageViewCI.subresourceRange.levelCount = m_Specification.Mips;
			imageViewCI.subresourceRange.baseArrayLayer = layer;
			imageViewCI.subresourceRange.layerCount = 1;
			imageViewCI.image = m_ImageInfo.Image;
			VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &imageViewCI, nullptr, &m_PerLayerImageViews[layer]));
		}
	}

	void VulkanImage2D::RTCreatePerSpecificLayerImageViews(const std::vector<uint32_t>& layerIndices) {
		SNOW_CORE_ASSERT(m_Specification.Layers > 1);

		auto vkDevice = VulkanContext::GetCurrentDevice();

		VkImageAspectFlags aspectMask = Render::Utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (m_Specification.Format == Render::ImageFormat::Depth24Stencil8)
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		const VkFormat vulkanFormat = Utils::GetVulkanFormat(m_Specification.Format);

		if (m_PerLayerImageViews.empty())
			m_PerLayerImageViews.resize(m_Specification.Layers);

		for (auto layer : layerIndices) {
			VkImageViewCreateInfo imageViewCI = {};
			imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCI.format = vulkanFormat;
			imageViewCI.flags = 0;
			imageViewCI.subresourceRange = {};
			imageViewCI.subresourceRange.aspectMask = aspectMask;
			imageViewCI.subresourceRange.baseMipLevel = 0;
			imageViewCI.subresourceRange.levelCount = m_Specification.Mips;
			imageViewCI.subresourceRange.baseArrayLayer = layer;
			imageViewCI.subresourceRange.layerCount = 1;
			imageViewCI.image = m_ImageInfo.Image;
			VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &imageViewCI, nullptr, &m_PerLayerImageViews[layer]));
		}
	}

	void VulkanImage2D::UpdateDescriptor() {
		if (m_Specification.Format == Render::ImageFormat::Depth24Stencil8 || m_Specification.Format == Render::ImageFormat::Depth32F)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
		else
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (m_Specification.Usage == Render::ImageUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		m_DescriptorImageInfo.imageView = m_ImageInfo.ImageView;
		m_DescriptorImageInfo.sampler = m_ImageInfo.Sampler;
	}

	const std::map<VkImage, WeakRef<VulkanImage2D>>& VulkanImage2D::GetImageRefs() {
		return s_ImageReferences;
	}
}