#pragma once

#include <vulkan/vulkan.h>

#include "Snow/Render/Image.h"


namespace Snow {
	namespace Utils {
		inline VkFormat GetVulkanFormat(Render::ImageFormat format) {
			switch (format) {
			case Render::ImageFormat::Red:	return VkFormat::VK_FORMAT_R8_UNORM;
			case Render::ImageFormat::RG:	return VkFormat::VK_FORMAT_R8G8_UNORM;
			case Render::ImageFormat::RGB:	return VkFormat::VK_FORMAT_R8G8B8_UNORM;
			case Render::ImageFormat::SRGB:	return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
			case Render::ImageFormat::RGBA:	return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			case Render::ImageFormat::RGBA16F:
			case Render::ImageFormat::RGBA32F:	return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
			case Render::ImageFormat::Depth32F:	return VkFormat::VK_FORMAT_D32_SFLOAT;
			case Render::ImageFormat::Depth24Stencil8:	return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
			}
			SNOW_CORE_ASSERT(false, "Format not defined");
			return VkFormat::VK_FORMAT_UNDEFINED;
		}

		inline bool IsDepthFormat(Render::ImageFormat format) {
			if (format == Render::ImageFormat::Depth32F || format == Render::ImageFormat::Depth24Stencil8)
				return true;
			
			return false;
		}

		inline VkImageUsageFlags GetVulkanImageUsage(Render::ImageUsage usage, Render::ImageFormat format) {
			VkImageUsageFlags result = VK_IMAGE_USAGE_SAMPLED_BIT;
			switch (usage) {
			case Render::ImageUsage::Attachment:	
				if(IsDepthFormat(format))
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