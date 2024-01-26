#pragma once


#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include <unordered_set>

#include "Snow/Core/Ref.h"

namespace Snow {
	class VulkanDevice : public RefCounted {
		struct QueueFamilyIndices;
	public:
		VulkanDevice();

		bool IsExtensionSupported(const std::string& extensionName) const;

		uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags props) const;
		VkQueue GetGraphicsQueue() { return m_Queue; }
		VkQueue GetComputeQueue() { return m_ComputeQueue; }

		VkCommandBuffer GetCommandBuffer(bool begin, bool compute = false);
		void FlushCommandBuffer(VkCommandBuffer commandBuffer);

		VkCommandBuffer CreateSecondaryCommandBuffer();

		VkPhysicalDevice GetPhysicalDevice() { return m_VulkanPhysicalDevice; }
		VkDevice GetVulkanDevice() { return m_VulkanDevice; }

		const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

		const VkPhysicalDeviceProperties& GetProperties() const { return m_VulkanPhysicalDeviceProperties; }

		VkFormat FindDepthFormat() const;

		VkFormat GetDepthFormat() const { return m_DepthFormat; }

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
		VkPhysicalDeviceProperties2 m_VulkanPhysicalDeviceProperties2;
		VkPhysicalDeviceFeatures m_VulkanPhysicalDeviceFeatures;
		VkPhysicalDeviceFeatures2 m_VulkanPhysicalDeviceFeatures2;
		VkPhysicalDeviceMemoryProperties m_VulkanPhysicalDeviceMemoryProperties;
		std::vector<VkQueueFamilyProperties> m_QueueFamiliesProperties;
		std::unordered_set<std::string> m_ExtensionProperties;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

		QueueFamilyIndices m_QueueFamilyIndices;

		VkQueue m_Queue, m_ComputeQueue;
		VkCommandPool m_CommandPool, m_ComputeCommandPool;

		VkDevice m_VulkanDevice;
	};

}