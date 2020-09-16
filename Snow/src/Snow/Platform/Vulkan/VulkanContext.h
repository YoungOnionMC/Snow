#include "Snow/Render/RenderContext.h"

#if defined(SNOW_WINDOW_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(SNOW_WINDOW_GLFW)
//#define VK_USE_PLATFOMR_XLIB_KHR
#define VK_USE_PLATFORM_XCB_KHR
#endif
#include <vulkan/vulkan.h>

#include "Snow/Platform/Vulkan/VulkanDevice.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Ref.h"

#include <vector>


namespace Snow {
    namespace Render {
        class VulkanContext : public Context {
        public:
            VulkanContext(const ContextSpecification& spec);
            ~VulkanContext();

            static VkInstance GetVulkanInstance() { return s_VulkanInstance; }

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }

            Ref<VulkanDevice> GetDevice() { return m_Device; }
            VulkanSwapChain GetSwapChain() {return m_SwapChain; }

            static VulkanContext* Get() {return static_cast<VulkanContext*>(Renderer::GetContext());}
            static Ref<VulkanDevice> GetCurrentDevice() { return Get()->GetDevice(); }
        private:
            void CreateInstance();



            static VkInstance s_VulkanInstance;
            

            Ref<VulkanDevice> m_Device;
            VulkanSwapChain m_SwapChain;

            VkResult m_Result;

            std::vector<const char*> m_InstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
            std::vector<VkExtensionProperties> m_Extensions;

            std::vector<const char*> m_InstanceLayers = { "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_standard_validation", /*"VK_LAYER_LUNARG_api_dump"*/ };


            ContextSpecification m_Specification;

        };
    }
}