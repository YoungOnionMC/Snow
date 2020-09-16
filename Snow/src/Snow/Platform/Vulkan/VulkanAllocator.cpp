#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanAllocator.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {
    namespace Render {
        VulkanAllocator::VulkanAllocator(const std::string& tag):
            m_Device(VulkanContext::GetCurrentDevice()), m_Tag(tag) {} 

        VulkanAllocator::VulkanAllocator(Ref<VulkanDevice>& device, const std::string& tag) :
            m_Device(device), m_Tag(tag) {}

        void VulkanAllocator::Allocate(VkMemoryRequirements reqs, VkDeviceMemory* dest, VkMemoryPropertyFlags flags) {
            VkMemoryAllocateInfo memAlloc = {};
            memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            memAlloc.allocationSize = reqs.size;
            
            memAlloc.memoryTypeIndex = m_Device->GetMemoryTypeIndex(reqs.memoryTypeBits, flags);
            vkAllocateMemory(m_Device->GetDevice(), &memAlloc, nullptr, dest);
        }
    }
}