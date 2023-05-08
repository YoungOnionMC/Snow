#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanImage.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanRenderer.h"



namespace Snow {

	static std::map<VkImage, WeakRef<VulkanImage2D>> s_ImageReferences;

	//using namespace Render;
	VulkanImage2D::VulkanImage2D(Render::ImageSpecification spec) :
		m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height) {
		
	}

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

		Render::Renderer::SubmitResourceFree([instance, info, layerViews = m_PerLayerImageViews]() mutable {
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
			allocator.DestroyBuffer(instance->m_StagingBuffer, instance->m_StagingBufferAllocation);
			instance->m_StagingBuffer = nullptr;
			instance->m_StagingBufferAllocation = nullptr;
		});
		m_ImageInfo.Image = nullptr;
		m_ImageInfo.ImageView = nullptr;
		m_ImageInfo.Sampler = nullptr;
		m_PerLayerImageViews.clear();
		

		
	}

	void VulkanImage2D::SetData(const void* data) {
		Ref<VulkanImage2D> instance = this;
		Render::Renderer::Submit([instance, data]() mutable {
			instance->RTSetData(data);
		});
	}

	void VulkanImage2D::RTSetData(const void* data) {
		Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();
		VkCommandBuffer cmdBuffer = vkDevice->GetCommandBuffer(true);
		VulkanAllocator allocator("VulkanImage2D");

		uint64_t size = m_Width * m_Height * Render::Utils::GetFormatSize(m_Specification.Format);
		if (!m_StagingBuffer) {
			VkBufferCreateInfo bufferCI = {};
			bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCI.size = size;
			bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			m_StagingBufferAllocation = allocator.AllocateBuffer(bufferCI, VMA_MEMORY_USAGE_CPU_TO_GPU, m_StagingBuffer);
			//vkCreateBuffer(vkDevice->GetVulkanDevice(), &bufferCI, nullptr, &m_StagingBuffer);
		}

		uint8_t* allocatedData = allocator.MapMemory<uint8_t>(m_StagingBufferAllocation);
		SNOW_CORE_ASSERT(data);
		
		//size_t allocSize = allocator.GetAllocationInfo(m_StagingBufferAllocation).size;
		//if(allocSize <= size)
		//SNOW_CORE_INFO("Alloc size {0}, data size {1}", allocSize, size);
		memcpy(allocatedData, data, size);
		allocator.UnmapMemory(m_StagingBufferAllocation);


		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		Render::Utils::InsertImageMemoryBarrier(cmdBuffer, m_ImageInfo.Image, 
			0, VK_ACCESS_TRANSFER_WRITE_BIT, 
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
			VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 
			subresourceRange);

		VkBufferImageCopy region = {};
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageExtent.width = m_Width;
		region.imageExtent.height = m_Height;
		region.imageExtent.depth = 1;
		region.bufferOffset = 0;

		vkCmdCopyBufferToImage(cmdBuffer, m_StagingBuffer, m_ImageInfo.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		if (m_Specification.Mips > 1) {
			Render::Utils::InsertImageMemoryBarrier(cmdBuffer, m_ImageInfo.Image,
				VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				subresourceRange);
		}
		else {
			Render::Utils::InsertImageMemoryBarrier(cmdBuffer, m_ImageInfo.Image,
				VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_DescriptorImageInfo.imageLayout,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				subresourceRange);
		}

		vkDevice->FlushCommandBuffer(cmdBuffer);
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

		VkFormat vulkanFormat = Render::Utils::GetVulkanFormat(m_Specification.Format);

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

	void VulkanImage2D::Resize(uint32_t width, uint32_t height) {
		if (m_ImageInfo.Image && m_Width == width && m_Height == height)
			return;

		m_Width = width;
		m_Height = height;
		m_Specification.Width = m_Width;
		m_Specification.Height = m_Height;

		Release();
		RTInvalidate();
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

		const VkFormat vulkanFormat = Render::Utils::GetVulkanFormat(m_Specification.Format);

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

		const VkFormat vulkanFormat = Render::Utils::GetVulkanFormat(m_Specification.Format);

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
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
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