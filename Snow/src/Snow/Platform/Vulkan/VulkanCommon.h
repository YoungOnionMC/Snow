#pragma once

#include <vulkan/vulkan.h>


static void VulkanErrorCheck(VkResult result) {
    if(result != VK_SUCCESS) 
        SNOW_CORE_ERROR("VkResult is '{0}' in {1}:{2}", result, __LINE__, __FILE__);
}

#define VKCheckError(f) { VulkanErrorCheck((f)); }