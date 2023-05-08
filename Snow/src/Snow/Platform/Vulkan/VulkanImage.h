#pragma once


#include "Snow/Render/Image.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Platform/Vulkan/VulkanAllocator.h"

#include "VulkanUtils.h"

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

		virtual void SetData(const void* data) override;
		void RTSetData(const void* data);
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

		std::vector<VkImageView> m_PerLayerImageViews;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
	};
}