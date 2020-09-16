#pragma once

#include <vulkan/vulkan.h>


#define VKCheckError(f) {\
    VkResult result = (f);\
    if(result != VK_SUCCESS) SNOW_CORE_ERROR("VkResult is '{0}' in {1}:{2}", result, __LINE__, __FILE__);\
}