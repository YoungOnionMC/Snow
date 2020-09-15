#pragma once

#include "Snow/Render/RenderDevice.h"

#include <vulkan/vulkan.h>

#include <unordered_set>

namespace Snow {
	namespace Render {
		class VulkanDevice : public Device {
		public:
			VulkanDevice(const DeviceSpecification& spec);

			bool IsExtensionSupported(const std::string& extensionName) const;

			VkCommandBuffer GetCommandBuffer(bool begin);
			void FlushCommandBuffer(VkCommandBuffer commandBuffer);

		private:
			void PickPhysicalDevice();
			bool IsDeviceSuitable(VkPhysicalDevice device);

			void GetDeviceExtensions();
			
			void CreateLogicalDevice();

			struct QueueFamilyIndices {
				int32_t Graphics = -1;
				int32_t Compute = -1;
				int32_t Transfer = -1;
			};
			QueueFamilyIndices GetQueueFamilyIndices(int flags);
			void CreateQueueInfos(int flags);

			


			VkPhysicalDevice m_VulkanPhysicalDevice;
			VkPhysicalDeviceProperties m_VulkanPhysicalDeviceProperties;
			VkPhysicalDeviceFeatures m_VulkanPhysicalDeviceFeatures;
			VkPhysicalDeviceMemoryProperties m_VulkanPhysicalDeviceMemoryProperties;
			std::vector<VkQueueFamilyProperties> m_QueueFamiliesProperties;
			std::unordered_set<std::string> m_ExtensionProperties;
			std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;

			QueueFamilyIndices m_QueueFamilyIndices;

			VkQueue m_Queue;
			VkCommandPool m_CommandPool;

			VkDevice m_VulkanDevice;

			VkResult m_Result;
		};
	}
}