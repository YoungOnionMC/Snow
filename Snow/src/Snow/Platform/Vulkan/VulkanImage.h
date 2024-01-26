#pragma once


#include "Snow/Render/Image.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Platform/Vulkan/VulkanAllocator.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {
	struct VulkanImageInfo {
		VkImage Image = nullptr;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
		VmaAllocation MemoryAllocation = nullptr;
	};

	
	class VulkanImage2D : public Render::Image2D {
	public:
		VulkanImage2D(Render::ImageSpecification spec);
		virtual ~VulkanImage2D() override;

		virtual void Invalidate() override;
		virtual void Release() override;

		virtual uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }

		virtual void SetData(Buffer& data) override;
		void RTSetData(Buffer& data);

		void CopyToHostBuffer(Buffer& data);
		virtual bool GetImageData() override { return m_ImageData; }

		void Resize(uint32_t width, uint32_t height) override;

		virtual Render::ImageSpecification& GetSpecification() override { return m_Specification; }
		virtual const Render::ImageSpecification& GetSpecification() const override { return m_Specification; }

		void RTInvalidate();

		virtual void CreatePerLayerImageViews() override;
		void RTCreatePerLayerImageViews();
		void RTCreatePerSpecificLayerImageViews(const std::vector<uint32_t>& layerIndices);

		VkImageView GetLayerImageView(uint32_t layer) {
			SNOW_CORE_ASSERT(layer < m_PerLayerImageViews.size());
			if (m_PerLayerImageViews.size() == 0)
				return nullptr;
			return m_PerLayerImageViews[layer];
		}

		VkImageView GetMipImageView(uint32_t mip) { return{}; }
		VkImageView RTGetMipImageView(uint32_t mip) { return {}; }

		virtual Buffer GetBuffer() const override { return m_ImageData; }
		virtual Buffer& GetBuffer() override { return m_ImageData; }

		virtual uint64_t GetHash() const override { return (uint64_t)m_ImageInfo.Image; }

		VulkanImageInfo& GetImageInfo() { return m_ImageInfo; }
		const VulkanImageInfo& GetImageInfo() const { return m_ImageInfo; }

		const VkDescriptorImageInfo& GetImageDescriptor() { return m_DescriptorImageInfo; }

		void UpdateDescriptor();

		static const std::map<VkImage, WeakRef<VulkanImage2D>>& GetImageRefs();
	private:
		Render::ImageSpecification m_Specification;

		uint32_t m_Width = 0, m_Height = 0;
		Buffer m_ImageData;
		VulkanImageInfo m_ImageInfo;

		VkBuffer m_StagingBuffer = nullptr;
		VmaAllocation m_StagingBufferAllocation;

		VkImageLayout m_CurrentLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		std::vector<VkImageView> m_PerLayerImageViews;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
	};

	namespace Utils {
		inline VkFormat GetVulkanFormat(Render::ImageFormat format) {
			switch (format) {
			case Render::ImageFormat::Red:	return VkFormat::VK_FORMAT_R8_UNORM;
			case Render::ImageFormat::RG:	return VkFormat::VK_FORMAT_R8G8_UNORM;
			case Render::ImageFormat::RGB:	return VkFormat::VK_FORMAT_R8G8B8_UNORM;
			case Render::ImageFormat::SRGB:	return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
			case Render::ImageFormat::RGBA:	return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			case Render::ImageFormat::RGBA16F:	return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
			case Render::ImageFormat::RGBA32F:	return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
			case Render::ImageFormat::Depth32F:	return VkFormat::VK_FORMAT_D32_SFLOAT;
				//case ImageFormat::DepthStencil:	return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
			case Render::ImageFormat::Depth24Stencil8:	return Snow::VulkanContext::GetCurrentDevice()->GetDepthFormat();
			}
			SNOW_CORE_ASSERT(false, "Format not defined");
			return VkFormat::VK_FORMAT_UNDEFINED;
		}

		inline VkImageUsageFlags GetVulkanImageUsage(Render::ImageUsage usage, Render::ImageFormat format) {
			VkImageUsageFlags result = VK_IMAGE_USAGE_SAMPLED_BIT;
			switch (usage) {
			case Render::ImageUsage::Attachment:
				if (ImageUtils::IsDepthFormat(format))
					result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				else
					result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				break;
				break;
			case Render::ImageUsage::Texture:	result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				break;
			case Render::ImageUsage::Storage:	result |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				break;
			}
			return result;
		}
	}
}