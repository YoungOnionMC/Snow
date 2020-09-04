#include "spch.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#include <GLFW/glfw3.h>

namespace Snow {
    namespace Render {
        VulkanContext::VulkanContext(const ContextSpecification& spec) :
            m_Specification(spec) {
            
            CreateInstance();
            //vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, nullptr);
            //vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, nullptr);
        }

        void VulkanContext::CreateInstance() {
            // Application information 
            /*
            VkApplicationInfo applicationInfo = {};
            applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            applicationInfo.pApplicationName = "ApplicationName";
            applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
            applicationInfo.pEngineName = "Snow Engine";
            applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 2);
            applicationInfo.apiVersion = VK_API_VERSION_1_1;

            // Extensions
            
            std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

            instanceExtensions.push_back("");
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            VkInstanceCreateInfo instanceCreateInfo = {};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &applicationInfo;
            instanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
            instanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;
            instanceCreateInfo.enabledLayerCount = 0;


            m_Result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstance);
            */
        }
    }
}