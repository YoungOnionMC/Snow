#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanDevice.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Render/Renderer.h"

namespace Snow {

	bool VulkanDevice::IsDeviceSuitable(VkPhysicalDevice device) {
		vkGetPhysicalDeviceProperties(device, &m_VulkanPhysicalDeviceProperties);
		
		vkGetPhysicalDeviceFeatures(device, &m_VulkanPhysicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(device, &m_VulkanPhysicalDeviceMemoryProperties);

		uint32_t familyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
		m_QueueFamiliesProperties.resize(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, m_QueueFamiliesProperties.data());

		return true;
	}

	void VulkanDevice::PickPhysicalDevice() {
		uint32_t deviceCount = 0;
		VKCheckError(vkEnumeratePhysicalDevices(VulkanContext::GetVulkanInstance(), &deviceCount, nullptr));

		if (deviceCount == 0)
			SNOW_CORE_ERROR("Failed to find a GPU supporting vulkan");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		VKCheckError(vkEnumeratePhysicalDevices(VulkanContext::GetVulkanInstance(), &deviceCount, devices.data()));

		for (auto device : devices) {
			if (IsDeviceSuitable(device))
				m_VulkanPhysicalDevice = device;
		}
	}

	void VulkanDevice::GetDeviceExtensions() {
		uint32_t extensionCount = 0;
		VKCheckError(vkEnumerateDeviceExtensionProperties(m_VulkanPhysicalDevice, nullptr, &extensionCount, nullptr));
		if (extensionCount > 0) {
			std::vector<VkExtensionProperties> extensions(extensionCount);
			if (vkEnumerateDeviceExtensionProperties(m_VulkanPhysicalDevice, nullptr, &extensionCount, &extensions.front()) == VK_SUCCESS) {
				for (const auto& ext : extensions) {
					m_ExtensionProperties.emplace(ext.extensionName);
					SNOW_CORE_INFO("	Found Vulkan device extension {0}", ext.extensionName);
				}
			}
		}

		

		//m_VulkanPhysicalDeviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		//vkGetPhysicalDeviceProperties2(m_VulkanPhysicalDevice, &m_VulkanPhysicalDeviceProperties2);
		//IsExtensionSupported(VK_KHR_RAY_TRACING_EXTENSION_NAME);
	}

	VulkanDevice::QueueFamilyIndices VulkanDevice::GetQueueFamilyIndices(int flags) {
		QueueFamilyIndices indices;
		if (flags & VK_QUEUE_COMPUTE_BIT) {
			for (uint32_t i = 0; i < m_QueueFamiliesProperties.size(); i++) {
				auto& queueFamilyProperties = m_QueueFamiliesProperties[i];
				if ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
					indices.Compute = i;
					break;
				}
			}
		}

		if (flags & VK_QUEUE_TRANSFER_BIT) {
			for (uint32_t i = 0; i < m_QueueFamiliesProperties.size(); i++) {
				auto& queueFamilyProperties = m_QueueFamiliesProperties[i];
				if ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
					indices.Transfer = i;
					break;
				}
			}
		}

		for (uint32_t i = 0; i < m_QueueFamiliesProperties.size(); i++) {
			if ((flags & VK_QUEUE_TRANSFER_BIT) && indices.Transfer == -1) {
				if (m_QueueFamiliesProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
					indices.Transfer = i;
			}

			if ((flags & VK_QUEUE_COMPUTE_BIT) && indices.Compute == -1) {
				if (m_QueueFamiliesProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
					indices.Compute = i;
			}

			if (flags & VK_QUEUE_GRAPHICS_BIT) {
				if (m_QueueFamiliesProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.Graphics = i;
			}
		}

		return indices;
	}

	void VulkanDevice::CreateQueueInfos(int flags) {
		static const float defaultQueuePriority(0.0f);
		if (flags & VK_QUEUE_GRAPHICS_BIT) {
			VkDeviceQueueCreateInfo queueInfo = {};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			m_QueueCreateInfos.push_back(queueInfo);
		}

		if (flags & VK_QUEUE_COMPUTE_BIT) {
			if (m_QueueFamilyIndices.Compute != m_QueueFamilyIndices.Graphics) {
				VkDeviceQueueCreateInfo queueInfo = {};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;
				m_QueueCreateInfos.push_back(queueInfo);
			}
		}

		if (flags & VK_QUEUE_TRANSFER_BIT) {
			if ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Graphics) && ((m_QueueFamilyIndices.Transfer != m_QueueFamilyIndices.Compute))) {
				VkDeviceQueueCreateInfo queueInfo = {};
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueInfo.queueFamilyIndex = m_QueueFamilyIndices.Transfer;
				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &defaultQueuePriority;
				m_QueueCreateInfos.push_back(queueInfo);
			}
		}
	}

	bool VulkanDevice::IsExtensionSupported(const std::string& extensionName) const {
		bool ExtensionFound = m_ExtensionProperties.find(extensionName) != m_ExtensionProperties.end();
		if (ExtensionFound) {
			SNOW_CORE_INFO("Found extension {0}", extensionName);
			return true;
		}
		else {
			SNOW_CORE_WARN("Could not find extension {0}", extensionName);
			return false;
		}
	}

	uint32_t VulkanDevice::GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags props) const {
		for (uint32_t i = 0; i < m_VulkanPhysicalDeviceMemoryProperties.memoryTypeCount; i++) {
			if ((typeBits & 1) == 1) {
				if ((m_VulkanPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & props) == props)
					return i;
			}
			typeBits >>= 1;
		}
		return UINT32_MAX;
	}

	void VulkanDevice::CreateLogicalDevice() {
		std::vector<const char*> deviceExtensions;
		if (IsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
			deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(m_QueueCreateInfos.size());
		deviceCreateInfo.pQueueCreateInfos = m_QueueCreateInfos.data();
		deviceCreateInfo.pEnabledFeatures = &m_VulkanPhysicalDeviceFeatures;

		
		if (IsExtensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) {
			deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
		}

		if (deviceExtensions.size() > 0) {
			deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
			deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		}

		VKCheckError(vkCreateDevice(m_VulkanPhysicalDevice, &deviceCreateInfo, nullptr, &m_VulkanDevice));
	}

	VkFormat VulkanDevice::FindDepthFormat() const {
		std::vector<VkFormat> depthFormats {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (auto& format : depthFormats) {
			VkFormatProperties formatProperties;
			vkGetPhysicalDeviceFormatProperties(m_VulkanPhysicalDevice, format, &formatProperties);
			if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				return format;
			}
		}
		return VK_FORMAT_UNDEFINED;
	}

	VulkanDevice::VulkanDevice() {

		PickPhysicalDevice();
		GetDeviceExtensions();


		int requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		m_QueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);
		CreateQueueInfos(requestedQueueTypes);

		m_DepthFormat = FindDepthFormat();
		SNOW_CORE_ASSERT(m_DepthFormat);

		CreateLogicalDevice();

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.Graphics;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VKCheckError(vkCreateCommandPool(m_VulkanDevice, &commandPoolCreateInfo, nullptr, &m_CommandPool));

		commandPoolCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.Compute;
		VKCheckError(vkCreateCommandPool(m_VulkanDevice, &commandPoolCreateInfo, nullptr, &m_ComputeCommandPool));

		vkGetDeviceQueue(m_VulkanDevice, m_QueueFamilyIndices.Graphics, 0, &m_Queue);
		vkGetDeviceQueue(m_VulkanDevice, m_QueueFamilyIndices.Compute, 0, &m_ComputeQueue);

		VkDebugUtilsObjectNameInfoEXT debugObjectName = {};
		debugObjectName.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;

		auto pfn_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(VulkanContext::GetVulkanInstance(), "vkSetDebugUtilsObjectNameEXT");
		pfn_vkSetDebugUtilsObjectNameEXT(m_VulkanDevice, &debugObjectName);
		//SNOW_CORE_ASSERT(vkCreateDebugReportCallbackEXT != NULL, "Creation of vulkan debug extension callback failed");
	}

	VkCommandBuffer VulkanDevice::GetCommandBuffer(bool begin, bool compute) {
		VkCommandBuffer cmdBuffer;
		VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
		cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufferAllocateInfo.commandPool = compute ? m_ComputeCommandPool : m_CommandPool;
		cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufferAllocateInfo.commandBufferCount = 1;
		VKCheckError(vkAllocateCommandBuffers(m_VulkanDevice, &cmdBufferAllocateInfo, &cmdBuffer));

		if (begin) {
			VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
			cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			VKCheckError(vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo));
		}
		return cmdBuffer;
	}

	void VulkanDevice::FlushCommandBuffer(VkCommandBuffer commandBuffer) {
		const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

		SNOW_CORE_ASSERT(commandBuffer != VK_NULL_HANDLE, "Invalid Command Buffer");
		VKCheckError(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;
		VkFence fence;
		VKCheckError(vkCreateFence(m_VulkanDevice, &fenceCreateInfo, nullptr, &fence));

		VKCheckError(vkQueueSubmit(m_Queue, 1, &submitInfo, fence));
		VKCheckError(vkWaitForFences(m_VulkanDevice, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT));

		vkDestroyFence(m_VulkanDevice, fence, nullptr);
		vkFreeCommandBuffers(m_VulkanDevice, m_CommandPool, 1, &commandBuffer);
	}

	VkCommandBuffer VulkanDevice::CreateSecondaryCommandBuffer() {
		VkCommandBuffer cmdBuffer;

		VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {};
		cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufferAllocateInfo.commandPool = m_CommandPool;
		cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		cmdBufferAllocateInfo.commandBufferCount = 1;
		VKCheckError(vkAllocateCommandBuffers(m_VulkanDevice, &cmdBufferAllocateInfo, &cmdBuffer));
		return cmdBuffer;
	}
}