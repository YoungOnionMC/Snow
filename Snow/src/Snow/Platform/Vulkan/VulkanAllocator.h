#pragma once

#include <string>
#include "Snow/Platform/Vulkan/VulkanCommon.h"
#include "Snow/Platform/Vulkan/VulkanDevice.h"

#include "vk_mem_alloc.h"

#include "Snow/Core/Ref.h"


namespace Snow {

    struct GPUMemoryStats {
        uint64_t Used = 0;
        uint64_t Free = 0;
    };

    class VulkanAllocator {
    public:
        VulkanAllocator() = default;
        VulkanAllocator(const std::string& tag);
        //VulkanAllocator(Ref<VulkanDevice>& device, const std::string& tag = "");

        //void Allocate(VkMemoryRequirements reqs, VkDeviceMemory* dest, VkMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VmaAllocation AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer);
        VmaAllocation AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage);
        void Free(VmaAllocation allocation);
        void DestroyImage(VkImage image, VmaAllocation allocation);
        void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation);

        template<typename T>
        T* MapMemory(VmaAllocation allocation) {
            T* mappedMemory;
            vmaMapMemory(VulkanAllocator::GetVMAAllocator(), allocation, (void**)&mappedMemory);
            return mappedMemory;
        }
        void UnmapMemory(VmaAllocation allocation);

        static void DumpStats();
        static GPUMemoryStats GetStats();

        static void Init(Ref<VulkanDevice> device);
        static void Shutdown();

        static VmaAllocator& GetVMAAllocator();
    private:
        std::string m_Tag;
    };
}