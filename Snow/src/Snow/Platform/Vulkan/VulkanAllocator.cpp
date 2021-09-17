#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanAllocator.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Utils/StringUtils.h"

#if SNOW_LOG_RENDERER_ALLOCATIONS
#define SNOW_ALLOCATOR_LOG(...) SNOW_CORE_TRACE(__VA_ARGS__)
#else
#define SNOW_ALLOCATOR_LOG(...)
#endif

namespace Snow {

    struct VulkanAllocatorData {
        VmaAllocator Allocator;
        uint64_t TotalAllocatedBytes = 0;
    };

    static VulkanAllocatorData* s_Data = nullptr;

    VulkanAllocator::VulkanAllocator(const std::string& tag) :
        m_Tag(tag) {}

#if 0
    void VulkanAllocator::Allocate(VkMemoryRequirements reqs, VkDeviceMemory* dest, VkMemoryPropertyFlags flags) {
        SNOW_CORE_ASSERT(m_Device);
        SNOW_CORE_TRACE("{0} allocating {1} bytes", m_Tag, (uint64_t)reqs.size);


        VkMemoryAllocateInfo memAlloc = {};
        memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memAlloc.allocationSize = reqs.size;
        memAlloc.memoryTypeIndex = m_Device->GetMemoryTypeIndex(reqs.memoryTypeBits, flags);
        VKCheckError(vkAllocateMemory(m_Device->GetVulkanDevice(), &memAlloc, nullptr, dest));
    }
#endif

    VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer) {
        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = usage;

        VmaAllocation allocation;
        vmaCreateBuffer(s_Data->Allocator, &bufferCreateInfo, &allocCreateInfo, &outBuffer, &allocation, nullptr);

        VmaAllocationInfo allocInfo{};
        vmaGetAllocationInfo(s_Data->Allocator, allocation, &allocInfo);
        SNOW_ALLOCATOR_LOG("VulkanAllocator ({0}): allocating buffer; size = {1}", m_Tag, Utils::BytesToString(allocInfo.size));

        {
            s_Data->TotalAllocatedBytes += allocInfo.size;
            SNOW_ALLOCATOR_LOG("VulkanAllocator ({0}): total allocated since start is {1}", m_Tag, Utils::BytesToString(s_Data->TotalAllocatedBytes));
        }

        return allocation;
    }

    VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage) {
        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = usage;

        VmaAllocation allocation;
        vmaCreateImage(s_Data->Allocator, &imageCreateInfo, &allocCreateInfo, &outImage, &allocation, nullptr);

        VmaAllocationInfo allocInfo{};
        vmaGetAllocationInfo(s_Data->Allocator, allocation, &allocInfo);
        SNOW_ALLOCATOR_LOG("VulkanAllocator ({0}): allocating image; size = {1}", m_Tag, Utils::BytesToString(allocInfo.size));

        {
            s_Data->TotalAllocatedBytes += allocInfo.size;
            SNOW_ALLOCATOR_LOG("VulkanAllocator ({0}): total allocated since start is {1}", m_Tag, Utils::BytesToString(s_Data->TotalAllocatedBytes));
        }

        return allocation;
    }

    void VulkanAllocator::Free(VmaAllocation allocation) {
        vmaFreeMemory(s_Data->Allocator, allocation);
    }

    void VulkanAllocator::DestroyImage(VkImage image, VmaAllocation allocation) {
        SNOW_CORE_ASSERT(image);
        SNOW_CORE_ASSERT(allocation);
        vmaDestroyImage(s_Data->Allocator, image, allocation);
    }

    void VulkanAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation) {
        SNOW_CORE_ASSERT(buffer);
        SNOW_CORE_ASSERT(allocation);
        vmaDestroyBuffer(s_Data->Allocator, buffer, allocation);
    }

    void VulkanAllocator::UnmapMemory(VmaAllocation allocation) {
        vmaUnmapMemory(s_Data->Allocator, allocation);
    }

    void VulkanAllocator::DumpStats() {
        VmaStats stats;
        vmaCalculateStats(s_Data->Allocator, &stats);

        uint64_t usedMemory = stats.memoryHeap[0].usedBytes;
        uint64_t freeMemory = stats.memoryHeap[0].unusedBytes;
    }

    GPUMemoryStats VulkanAllocator::GetStats() {
        VmaStats stats;
        vmaCalculateStats(s_Data->Allocator, &stats);

        uint64_t usedMemory = stats.total.usedBytes;
        uint64_t freeMemory = stats.total.unusedBytes;

        return { usedMemory, freeMemory };
    }

    void VulkanAllocator::Init(Ref<VulkanDevice> device) {
        s_Data = new VulkanAllocatorData();

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.physicalDevice = device->GetPhysicalDevice();
        allocatorInfo.device = device->GetVulkanDevice();
        allocatorInfo.instance = VulkanContext::GetVulkanInstance();

        vmaCreateAllocator(&allocatorInfo, &s_Data->Allocator);
    }

    void VulkanAllocator::Shutdown() {
        vmaDestroyAllocator(s_Data->Allocator);

        delete s_Data;
        s_Data = nullptr;
    }

    VmaAllocator& VulkanAllocator::GetVMAAllocator() {
        return s_Data->Allocator;
    }
}