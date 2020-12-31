#pragma once

#include <vulkan/vulkan.h>


static void VulkanErrorCheck(VkResult result) {
    if(result != VK_SUCCESS) 
        SNOW_CORE_ERROR("VkResult is '{0}' in {1}:{2}", result, __LINE__, __FILE__);
}

#define VKCheckError(f) { VulkanErrorCheck((f)); }




template<class T>
static void ZeroInitVulkanStruct(T& s, int32_t ty) {
    static_assert(sizeof(T::sType) == sizeof(int32_t));
    (int32_t&)s.sType = ty;
    ZeroMemory(((uint8_t*)&s) + sizeof(ty), sizeof(T) - sizeof(ty));
}