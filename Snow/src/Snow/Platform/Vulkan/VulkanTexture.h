#pragma once

#include "Snow/Render/Texture.h"

#include "Snow/Platform/Vulkan/VulkanImage.h"

namespace Snow {
	class VulkanTexture2D : public Render::Texture2D {
	public:
		VulkanTexture2D(const std::string& path, Render::TextureProperties props);
		VulkanTexture2D(Render::ImageFormat format, uint32_t width, uint32_t height, const void* data, Render::TextureProperties props);
		virtual ~VulkanTexture2D() {}

		void Invalidate();

		virtual Render::ImageFormat GetFormat() const override { return m_Format; }
		virtual uint32_t GetWidth() const override { return m_Image->GetWidth(); }
		virtual uint32_t GetHeight() const override { return m_Image->GetHeight(); }

		virtual uint32_t GetMipLevelCount() const override;
		virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const override;

		virtual bool Loaded() const override { return false; } //m_Image->GetImageData();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual Ref<Render::Image2D> GetImage() const override { return m_Image; }
		const VkDescriptorImageInfo& GetVulkanDescriptorInfo() const { return m_Image.As<VulkanImage2D>()->GetImageDescriptor(); }

		//void Lock() override;
		//void Unlock() override;

		void GenerateMips();

		virtual uint64_t GetHash() const override { return (uint64_t)m_Image; }

		virtual const std::string& GetPath() const override { return m_Path; }
	private:

		std::string m_Path;
		
		Render::TextureProperties m_Properties;

		uint32_t m_Width, m_Height;

		Buffer m_ImageData = {};
		Ref<Render::Image2D> m_Image;

		Render::ImageFormat m_Format = Render::ImageFormat::None;
	};

	class VulkanTextureCube : public Render::TextureCube {
	public:
		VulkanTextureCube(Render::ImageFormat format, uint32_t width, uint32_t height, const void* data, Render::TextureProperties properties);
		~VulkanTextureCube();

		virtual Render::ImageFormat GetFormat() const override { return m_Format; }
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		VkImageView CreateImageViewSingleMip(uint32_t mip);

		virtual uint32_t GetMipLevelCount() const override;
		virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const override;

		//Ref<Render::Image2D> GetImage() const { return m_Image; }
		const VkDescriptorImageInfo& GetVulkanDescriptorInfo() const { return m_DescriptorImageInfo; }

		virtual uint64_t GetHash() const override { return (uint64_t)m_Image; }
		virtual void Bind(uint32_t slot = 0) const override {}


		void GenerateMips(bool readOnly = false);
	private:
		void Invalidate();


		std::string m_Path;

		Render::ImageFormat m_Format = Render::ImageFormat::None;
		uint32_t m_Width, m_Height;
		Render::TextureProperties m_Properties;

		Buffer m_ImageData;

		VkImage m_Image = nullptr;
		VkDescriptorImageInfo m_DescriptorImageInfo;

		VmaAllocation m_MemoryAlloc;

	};
}