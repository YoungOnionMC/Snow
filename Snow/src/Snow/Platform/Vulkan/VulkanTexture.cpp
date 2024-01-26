#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanTexture.h"

#include <stb_image.h>

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanImage.h"
#include "Snow/Platform/Vulkan/VulkanRenderer.h"

#include "Snow/Render/Renderer.h"

namespace Snow {

	namespace Render::Utils {
		static VkFilter VulkanSamplerFilter(Render::TextureFilter filter) {
			switch (filter) {
			case Render::TextureFilter::Linear:	return VK_FILTER_LINEAR;
			case Render::TextureFilter::Nearest:	return VK_FILTER_NEAREST;
			}
			SNOW_CORE_ASSERT(false, "unknown filter mode");
			return (VkFilter)0;
		}

		static VkSamplerAddressMode VulkanSamplerWrap(Render::TextureWrap wrap) {
			switch (wrap) {
			case Render::TextureWrap::Clamp:	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case Render::TextureWrap::Repeat:	return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			}
			SNOW_CORE_ASSERT(false, "unknown wrap mode");
			return (VkSamplerAddressMode)0;
		}

		static size_t GetMemorySize(Render::ImageFormat format, uint32_t width, uint32_t height) {
			switch (format) {
			case Render::ImageFormat::RGBA:	return width * height * 4;
			case Render::ImageFormat::RGBA32F:	return width * height * 4 * sizeof(float);
			}
			SNOW_CORE_ASSERT(false);
			return 0;
		}
	}

	VulkanTexture2D::VulkanTexture2D(const std::string& path, Render::TextureProperties props) :
		m_Path(path), m_Properties(props) {
		int width, height, channels;
		if (stbi_is_hdr(path.c_str())) {
			m_ImageData.Data = (byte*)stbi_loadf(path.c_str(), &width, &height, &channels, 4);
			m_ImageData.Size = width * height * 4 * sizeof(float);
			m_Format = Render::ImageFormat::RGBA32F;
		}
		else {
			m_ImageData.Data = (byte*)stbi_load(path.c_str(), &width, &height, &channels, 4);
			m_ImageData.Size = width * height * 4;
			m_Format = Render::ImageFormat::RGBA;
		}

		SNOW_CORE_ASSERT(m_ImageData.Data, "Failed to load image");
		if (!m_ImageData.Data)
			return;

		m_Width = width;
		m_Height = height;

		Render::ImageSpecification imageSpec;
		imageSpec.Format = m_Format;
		imageSpec.Width = m_Width;
		imageSpec.Height = m_Height;
		imageSpec.Mips = GetMipLevelCount();
		m_Image = Render::Image2D::Create(imageSpec);

		SNOW_CORE_ASSERT(m_Format != Render::ImageFormat::None);

		Ref<VulkanTexture2D> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->Invalidate();
		});

	}

	VulkanTexture2D::VulkanTexture2D(Render::ImageFormat format, uint32_t width, uint32_t height, const void* data, Render::TextureProperties props) :
		m_Properties(props), m_Format(format) {
		m_Width = width;
		m_Height = height;
		
		size_t size = Render::Utils::GetMemorySize(format, width, height);

		if (data) {
			m_ImageData.Allocate(size);
			memcpy(m_ImageData.Data, data, m_ImageData.Size);
		}

		Render::ImageSpecification imageSpec;
		imageSpec.Format = m_Format;
		imageSpec.Width = m_Width;
		imageSpec.Height = m_Height;
		imageSpec.Mips = GetMipLevelCount();
		if (props.Storage)
			imageSpec.Usage = Render::ImageUsage::Storage;
		m_Image = Render::Image2D::Create(imageSpec);

		SNOW_CORE_ASSERT(m_Format != Render::ImageFormat::None);

		Ref<VulkanTexture2D> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->Invalidate();
		});
	}

	void VulkanTexture2D::Invalidate() {
		auto vkDevice = VulkanContext::GetCurrentDevice();
		
		m_Image->Release();

		uint32_t mipCount = m_Properties.GenerateMips ? GetMipLevelCount() : 1;

		Render::ImageSpecification& imageSpec = m_Image->GetSpecification();
		imageSpec.Format = m_Format;
		imageSpec.Width = m_Width;
		imageSpec.Height = m_Height;
		imageSpec.Mips = mipCount;
		if (!m_ImageData)
			imageSpec.Usage = Render::ImageUsage::Storage;

		Ref<VulkanImage2D> image = m_Image.As<VulkanImage2D>();
		image->RTInvalidate();

		auto& info = image->GetImageInfo();

		if (m_ImageData) {
			VkCommandBuffer copyCmdBuffer = vkDevice->GetCommandBuffer(true);
			VulkanAllocator allocator("VulkanImage2D");

			VkDeviceSize size = m_ImageData.Size;
			//uint64_t bufferSize = m_Specification.Width * m_Specification.Height * ImageUtils::GetFormatSize(m_Specification.Format);

			// create staging buffer
			VkBufferCreateInfo bufferCI = {};
			bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCI.size = size;
			bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkBuffer stagingBuffer;

			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCI, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			// copy the data to staging buffer
			uint8_t* dstData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			SNOW_ASSERT(m_ImageData.Data);
			memcpy(dstData, m_ImageData.Data, size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 1;
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			// Transition the image to transfer target, to copy the buffer data to it
			Render::Utils::InsertImageMemoryBarrier(copyCmdBuffer, image->GetImageInfo().Image,
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

			vkCmdCopyBufferToImage(copyCmdBuffer, stagingBuffer, image->GetImageInfo().Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			if (m_Properties.GenerateMips) {
				Render::Utils::InsertImageMemoryBarrier(copyCmdBuffer, image->GetImageInfo().Image,
					VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					subresourceRange);
			}
			else {
				Render::Utils::InsertImageMemoryBarrier(copyCmdBuffer, image->GetImageInfo().Image,
					VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					subresourceRange);
			}
			vkDevice->FlushCommandBuffer(copyCmdBuffer);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
		}
		else {
			VkCommandBuffer transitionCommandBuffer = vkDevice->GetCommandBuffer(true);
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.levelCount = GetMipLevelCount();
			subresourceRange.layerCount = 1;
			Render::Utils::SetImageLayout(transitionCommandBuffer, info.Image, VK_IMAGE_LAYOUT_UNDEFINED, image->GetImageDescriptor().imageLayout, subresourceRange);
			vkDevice->FlushCommandBuffer(transitionCommandBuffer);
		}

		VkSamplerCreateInfo sampler{};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.magFilter = Render::Utils::VulkanSamplerFilter(m_Properties.SamplerFilter);
		sampler.minFilter = Render::Utils::VulkanSamplerFilter(m_Properties.SamplerFilter);
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.addressModeV = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.addressModeW = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)mipCount;
		sampler.maxAnisotropy = 1.0f;
		sampler.anisotropyEnable = VK_FALSE;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VKCheckError(vkCreateSampler(vkDevice->GetVulkanDevice(), &sampler, nullptr, &info.Sampler));
		image->UpdateDescriptor();

		if (!m_Properties.Storage) {
			VkImageViewCreateInfo view{};
			view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view.format = Snow::Utils::GetVulkanFormat(m_Format);
			view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view.subresourceRange.baseMipLevel = 0;
			view.subresourceRange.baseArrayLayer = 0;
			view.subresourceRange.layerCount = 1;
			view.subresourceRange.levelCount = mipCount;
			view.image = info.Image;
			VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &view, nullptr, &info.ImageView));

			image->UpdateDescriptor();
		}

		if (m_ImageData && m_Properties.GenerateMips && mipCount > 1)
			GenerateMips();
	}

	void VulkanTexture2D::Bind(uint32_t slot) const {

	}

	uint32_t VulkanTexture2D::GetMipLevelCount() const {
		return Snow::ImageUtils::CalculateMipCount(m_Width, m_Height);
	}

	std::pair<uint32_t, uint32_t> VulkanTexture2D::GetMipSize(uint32_t mip) const {
		uint32_t width = m_Width;
		uint32_t height = m_Height;
		while (mip != 0) {
			width /= 2;
			height /= 2;
			mip--;
		}
		return { width, height };
	}

	void VulkanTexture2D::GenerateMips() {
		auto vkDevice = VulkanContext::GetCurrentDevice();

		Ref<VulkanImage2D> image = m_Image.As<VulkanImage2D>();
		const auto& info = image->GetImageInfo();

		const VkCommandBuffer blitCmd = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = info.Image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		const auto mipLevels = GetMipLevelCount();
		for (uint32_t i = 1; i < mipLevels; i++) {
			VkImageBlit imageBlit{};

			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.layerCount = 1;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcOffsets[1].x = int32_t(m_Width >> (i - 1));
			imageBlit.srcOffsets[1].y = int32_t(m_Height >> (i - 1));
			imageBlit.srcOffsets[1].z = 1;

			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.layerCount = 1;
			imageBlit.dstSubresource.mipLevel = i;
			imageBlit.dstOffsets[1].x = int32_t(m_Width >> i);
			imageBlit.dstOffsets[1].y = int32_t(m_Height >> i);
			imageBlit.dstOffsets[1].z = 1;

			VkImageSubresourceRange mipSubRange = {};
			mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			mipSubRange.baseMipLevel = i;
			mipSubRange.levelCount = 1;
			mipSubRange.layerCount = 1;

			Render::Utils::InsertImageMemoryBarrier(blitCmd, info.Image,
				0, VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, mipSubRange);

			vkCmdBlitImage(blitCmd, 
				info.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
				info.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
				&imageBlit, VK_FILTER_LINEAR);

			Render::Utils::InsertImageMemoryBarrier(blitCmd, info.Image,
				VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, mipSubRange);
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.layerCount = 1;
		subresourceRange.levelCount = mipLevels;

		Render::Utils::InsertImageMemoryBarrier(blitCmd, info.Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, subresourceRange);

		VulkanContext::GetCurrentDevice()->FlushCommandBuffer(blitCmd);
	}

	VulkanTextureCube::VulkanTextureCube(Render::ImageFormat format, uint32_t width, uint32_t height, const void* data, Render::TextureProperties properties) :
		m_Format(format), m_Width(width), m_Height(height), m_Properties(properties) {
		if (data) {
			size_t size = Render::Utils::GetMemorySize(format, width, height) * 6;
			m_ImageData = Buffer::Copy((void*)data, size);
		}
		
		Ref<VulkanTextureCube> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->Invalidate();
		});
	}

	VulkanTextureCube::~VulkanTextureCube() {
		VkImageView imageView = m_DescriptorImageInfo.imageView;
		VkSampler sampler = m_DescriptorImageInfo.sampler;

		VkImage image = m_Image;
		VmaAllocation allocation = m_MemoryAlloc;

		Renderer::SubmitResourceFree([imageView, sampler, image, allocation]() {
			auto vkDevice = VulkanContext::GetCurrentDevice();
			vkDestroyImageView(vkDevice->GetVulkanDevice(), imageView, nullptr);
			vkDestroySampler(vkDevice->GetVulkanDevice(), sampler, nullptr);

			VulkanAllocator allocator("TextureCube");
			allocator.DestroyImage(image, allocation);
		});
	}

	void VulkanTextureCube::Invalidate() {
		auto device = VulkanContext::GetCurrentDevice();

		VkFormat format = Snow::Utils::GetVulkanFormat(m_Format);
		uint32_t mipCount = GetMipLevelCount();

		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		VulkanAllocator allocator("TextureCube");

		VkImageCreateInfo imageCI = {};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = format;
		imageCI.mipLevels = mipCount;
		imageCI.arrayLayers = 6;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCI.extent = { m_Width, m_Height, 1 };
		imageCI.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
		imageCI.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		m_MemoryAlloc = allocator.AllocateImage(imageCI, VMA_MEMORY_USAGE_GPU_ONLY, m_Image);

		m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		if (m_ImageData) {
			// Create staging buffer
			VkBufferCreateInfo bufferCI{};
			bufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCI.size = m_ImageData.Size;
			bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			bufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCI, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			uint8_t* dstData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
			memcpy(dstData, m_ImageData.Data, m_ImageData.Size);
			allocator.UnmapMemory(stagingBufferAllocation);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

			VkImageSubresourceRange subresourceRange = {};

			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.layerCount = 6;

			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.image = m_Image;
			imageMemoryBarrier.subresourceRange = subresourceRange;
			imageMemoryBarrier.srcAccessMask = 0;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			vkCmdPipelineBarrier(copyCmd,
				VK_PIPELINE_STAGE_HOST_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);

			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = 0;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 6;
			bufferCopyRegion.imageExtent.width = m_Width;
			bufferCopyRegion.imageExtent.height = m_Height;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = 0;

			vkCmdCopyBufferToImage(copyCmd, stagingBuffer,
				m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &bufferCopyRegion);


			Render::Utils::InsertImageMemoryBarrier(copyCmd, m_Image,
				VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				subresourceRange);

			device->FlushCommandBuffer(copyCmd);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
		}

		VkCommandBuffer layoutCmd = device->GetCommandBuffer(true);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = mipCount;
		subresourceRange.layerCount = 6;

		Render::Utils::SetImageLayout(layoutCmd, m_Image, VK_IMAGE_LAYOUT_UNDEFINED, m_DescriptorImageInfo.imageLayout, subresourceRange);

		device->FlushCommandBuffer(layoutCmd);

		VkSamplerCreateInfo sampler{};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.magFilter = Render::Utils::VulkanSamplerFilter(m_Properties.SamplerFilter);
		sampler.minFilter = Render::Utils::VulkanSamplerFilter(m_Properties.SamplerFilter);
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.addressModeV = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.addressModeW = Render::Utils::VulkanSamplerWrap(m_Properties.SamplerWrap);
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)mipCount;
		sampler.maxAnisotropy = 1.0f;
		sampler.anisotropyEnable = VK_FALSE;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		VKCheckError(vkCreateSampler(device->GetVulkanDevice(), &sampler, nullptr, &m_DescriptorImageInfo.sampler));

		VkImageViewCreateInfo view{};
		view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		view.format = format;
		view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

		view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 6;
		view.subresourceRange.levelCount = mipCount;
		view.image = m_Image;
		VKCheckError(vkCreateImageView(device->GetVulkanDevice(), &view, nullptr, &m_DescriptorImageInfo.imageView));
	}

	uint32_t VulkanTextureCube::GetMipLevelCount() const {
		return Snow::ImageUtils::CalculateMipCount(m_Width, m_Height);
	}

	std::pair<uint32_t, uint32_t> VulkanTextureCube::GetMipSize(uint32_t mip) const {
		uint32_t width = m_Width;
		uint32_t height = m_Height;
		while (mip != 0) {
			width /= 2;
			height /= 2;
			mip--;
		}
		return { width, height };
	}

	VkImageView VulkanTextureCube::CreateImageViewSingleMip(uint32_t mip) {
		auto vkDevice = VulkanContext::GetCurrentDevice();
		VkFormat format = Snow::Utils::GetVulkanFormat(m_Format);

		VkImageViewCreateInfo view{};
		view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		view.format = format;
		view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

		view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view.subresourceRange.baseMipLevel = mip;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 6;
		view.subresourceRange.levelCount = 1;
		view.image = m_Image;

		VkImageView result;
		VKCheckError(vkCreateImageView(vkDevice->GetVulkanDevice(), &view, nullptr, &result));
		return result;
	}

	void VulkanTextureCube::GenerateMips(bool readOnly) {
		auto vkDevice = VulkanContext::GetCurrentDevice();

		const VkCommandBuffer blitCmd = VulkanContext::GetCurrentDevice()->GetCommandBuffer(true);

		uint32_t mipLevels = GetMipLevelCount();
		for (uint32_t face = 0; face < 6; face++) {
			VkImageSubresourceRange mipSubRange{};
			mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			mipSubRange.baseMipLevel = 0;
			mipSubRange.baseArrayLayer = face;
			mipSubRange.levelCount = 1;
			mipSubRange.layerCount = 1;

			Render::Utils::InsertImageMemoryBarrier(blitCmd, m_Image, 
				0, VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, mipSubRange);
		}

		for (uint32_t i = 1; i < mipLevels; i++) {
			for (uint32_t face = 0; face < 6; face++) {
				VkImageBlit imageBlit{};

				imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageBlit.srcSubresource.layerCount = 1;
				imageBlit.srcSubresource.mipLevel = i - 1;
				imageBlit.srcSubresource.baseArrayLayer = face;
				imageBlit.srcOffsets[1].x = int32_t(m_Width >> (i - 1));
				imageBlit.srcOffsets[1].y = int32_t(m_Height >> (i - 1));
				imageBlit.srcOffsets[1].z = 1;

				imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageBlit.dstSubresource.layerCount = 1;
				imageBlit.dstSubresource.mipLevel = i;
				imageBlit.dstSubresource.baseArrayLayer = face;
				imageBlit.dstOffsets[1].x = int32_t(m_Width >> i);
				imageBlit.dstOffsets[1].y = int32_t(m_Height >> i);
				imageBlit.dstOffsets[1].z = 1;

				VkImageSubresourceRange mipSubRange = {};
				mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				mipSubRange.baseMipLevel = i;
				mipSubRange.baseArrayLayer = face;
				mipSubRange.levelCount = 1;
				mipSubRange.layerCount = 1;

				Render::Utils::InsertImageMemoryBarrier(blitCmd, m_Image,
					0, VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, mipSubRange);

				vkCmdBlitImage(blitCmd, m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
					&imageBlit, VK_FILTER_LINEAR);

				Render::Utils::InsertImageMemoryBarrier(blitCmd, m_Image,
					VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, mipSubRange);
			}
		}

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.layerCount = 6;
		subresourceRange.levelCount = mipLevels;

		Render::Utils::InsertImageMemoryBarrier(blitCmd, m_Image,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readOnly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, subresourceRange);

		VulkanContext::GetCurrentDevice()->FlushCommandBuffer(blitCmd);

		m_DescriptorImageInfo.imageLayout = readOnly ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_GENERAL;
	}
}