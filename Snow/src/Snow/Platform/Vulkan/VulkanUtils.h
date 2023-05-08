#pragma once

#include <vulkan/vulkan.h>

#include "Snow/Render/Image.h"


namespace Snow {
	namespace Render::Utils {
		inline VkFormat GetVulkanFormat(ImageFormat format) {
			switch (format) {
			case ImageFormat::Red:	return VkFormat::VK_FORMAT_R8_UNORM;
			case ImageFormat::RG:	return VkFormat::VK_FORMAT_R8G8_UNORM;
			case ImageFormat::RGB:	return VkFormat::VK_FORMAT_R8G8B8_UNORM;
			case ImageFormat::SRGB:	return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
			case ImageFormat::RGBA:	return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			case ImageFormat::RGBA16F:	return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
			case ImageFormat::RGBA32F:	return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
			case ImageFormat::Depth32F:	return VkFormat::VK_FORMAT_D32_SFLOAT;
			//case ImageFormat::DepthStencil:	return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
			case ImageFormat::Depth24Stencil8:	return VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT;
			}
			SNOW_CORE_ASSERT(false, "Format not defined");
			return VkFormat::VK_FORMAT_UNDEFINED;
		}

		inline VkImageUsageFlags GetVulkanImageUsage(ImageUsage usage, ImageFormat format) {
			VkImageUsageFlags result = VK_IMAGE_USAGE_SAMPLED_BIT;
			switch (usage) {
			case ImageUsage::Attachment:	
				if(IsDepthFormat(format))
					result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				else
					result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
				break;
				break;
			case ImageUsage::Texture:	result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				break;
			case ImageUsage::Storage:	result |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				break;
			}
			return result;
		}
	}
}