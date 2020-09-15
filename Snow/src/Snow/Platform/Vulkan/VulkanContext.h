#include "Snow/Render/RenderContext.h"

#if defined(SNOW_WINDOW_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#elif dfeined(SNOW_WINDOW_XLIB)
#define VK_USE_PLATFOMR_XLIB_KHR
#endif
#include <vulkan/vulkan.h>

#include "Snow/Platform/Vulkan/VulkanDevice.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

#include <vector>


namespace Snow {
    namespace Render {
        class VulkanContext : public Context {
        public:
            VulkanContext(const ContextSpecification& spec);
            ~VulkanContext();

            static VkInstance GetVulkanInstance() { return s_VulkanInstance; }

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }
        private:
            void CreateInstance();
            void CreateSurface();



            static VkInstance s_VulkanInstance;
            VkSurfaceKHR m_VulkanSurface;

            VulkanDevice* m_Device;
            VulkanSwapChain* m_SwapChain;

            VkResult m_Result;

            std::vector<const char*> m_InstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
            std::vector<VkExtensionProperties> m_Extensions;

            std::vector<const char*> m_InstanceLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_standard_validation", "VK_LAYER_LUNARG_api_dump" };


            ContextSpecification m_Specification;

        };
    }
}