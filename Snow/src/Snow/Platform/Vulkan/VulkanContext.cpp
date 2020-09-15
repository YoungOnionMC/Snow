#include "spch.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Snow {
    namespace Render {
        VulkanContext::VulkanContext(const ContextSpecification& spec) :
            m_Specification(spec) {

                SNOW_CORE_TRACE("Creating Vulkan Context");
            
            CreateInstance();
            //vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, nullptr);
            //vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, nullptr);
        }

        VulkanContext::~VulkanContext() {
            vkDestroyInstance(m_VulkanInstance, nullptr);
        }

        void VulkanContext::CreateInstance() {
            // Application information 
            
            VkApplicationInfo applicationInfo = {};
            applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            applicationInfo.pApplicationName = "ApplicationName";
            applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
            applicationInfo.pEngineName = "Snow Engine";
            applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 2);
            applicationInfo.apiVersion = VK_API_VERSION_1_2;
 
            // Extensions

            SNOW_CORE_TRACE("IS VULKAN SUPPORTED? {0}", glfwVulkanSupported());
            //SNOW_CORE_TRACE("Finding Vulkan Extensions");

            
            
#if defined(_WIN32)
            SNOW_CORE_TRACE("1");
            m_InstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
            SNOW_CORE_TRACE("2");
	        m_InstanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
            SNOW_CORE_TRACE("3");
	        m_InstanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
            SNOW_CORE_TRACE("4");
	        m_InstanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
            
#elif defined(VK_USE_PLATFORM_XCB_KHR)
            SNOW_CORE_TRACE("5");
	        m_InstanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
            SNOW_CORE_TRACE("6");
	        m_InstanceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
            SNOW_CORE_TRACE("7");
	        m_InstanceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#else
            SNOW_CORE_TRACE("8");
#endif

            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            m_Extensions.resize(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_Extensions.data());
            SNOW_CORE_TRACE("Number of Extensions, {0}", extensionCount);
            for(const auto& extension : m_Extensions)
                SNOW_CORE_TRACE("Extension {0}", extension.extensionName);


            SNOW_CORE_TRACE("Creating Vulkan Instance Info");
            VkInstanceCreateInfo instanceCreateInfo = {};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &applicationInfo;
            instanceCreateInfo.enabledExtensionCount = 0;
            instanceCreateInfo.ppEnabledExtensionNames = 0;
            instanceCreateInfo.enabledLayerCount = 0;


            m_Result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstance);
            SNOW_CORE_TRACE("VkResult: {0}", m_Result);
        }

        
    }
}