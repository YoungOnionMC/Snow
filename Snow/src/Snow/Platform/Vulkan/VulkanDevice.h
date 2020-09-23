#pragma once


#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include <unordered_set>

#include "Snow/Core/Ref.h"

namespace Snow {
	namespace Render {
		class VulkanDevice : public RefCounted {
		public:
			VulkanDevice();

			bool IsExtensionSupported(const std::string& extensionName) const;

			uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags props) const;
			VkQueue GetQueue() { return m_Queue; }

			VkCommandBuffer GetCommandBuffer(bool begin);
			void FlushCommandBuffer(VkCommandBuffer commandBuffer);

			VkPhysicalDevice GetPhysicalDevice() { return m_VulkanPhysicalDevice; }
			VkDevice GetDevice() { return m_VulkanDevice; }


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
		};
	}
}