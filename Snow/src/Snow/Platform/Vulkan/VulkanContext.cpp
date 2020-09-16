#include "spch.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#elif defined(SNOW_WINDOW_WIN32)
#include <Windows.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Render {

        VkInstance VulkanContext::s_VulkanInstance = nullptr;

        VulkanContext::VulkanContext(const ContextSpecification& spec) :
            m_Specification(spec) {

            SNOW_CORE_INFO("Creating Vulkan Context");
#if defined(SNOW_WINDOW_GLFW)
            glfwMakeContextCurrent((GLFWwindow*)m_Specification.WindowHandle);
            if(!glfwVulkanSupported())
                SNOW_CORE_ERROR("GLFW does not support vulkan");
#endif
            
            CreateInstance();

            DeviceSpecification deviceSpec;
            m_Device = Ref<VulkanDevice>::Create();

            SwapChainSpecification swapchainSpec;
            m_SwapChain.Init(s_VulkanInstance, m_Device);

            uint32_t width = 1080, height = 720;
            m_SwapChain.Create(&width, &height);
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
            SNOW_CORE_ERROR("Vulkan error {0}", pCallbackData->pMessage);
            return VK_FALSE;
        }

        VulkanContext::~VulkanContext() {
            vkDestroyInstance(s_VulkanInstance, nullptr);
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
 
            
            //SNOW_CORE_TRACE("Finding Vulkan Extensions");

            
            
#if defined(SNOW_WINDOW_WIN32)
            m_InstanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(SNOW_WINDOW_GLFW)
            uint32_t extensionCount = 0;
            const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

            for(uint32_t i=0; i< extensionCount; i++){
                m_InstanceExtensions.push_back(extensions[i]);
                //SNOW_CORE_TRACE("Extensions {0}", extensions[i]);
            }
            //m_InstanceExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

            //m_InstanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            //m_InstanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
            
           // m_InstanceExtensions.push_back();
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
#endif

            VkInstanceCreateInfo instanceCreateInfo = {};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &applicationInfo;

            VkDebugUtilsMessengerCreateInfoEXT debugMessageCreateInfo;
            if (m_InstanceExtensions.size() > 0) {
                instanceCreateInfo.enabledExtensionCount = m_InstanceExtensions.size();
                instanceCreateInfo.ppEnabledExtensionNames = m_InstanceExtensions.data();

                debugMessageCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debugMessageCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                debugMessageCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                debugMessageCreateInfo.pfnUserCallback = debugCallback;
                SNOW_CORE_INFO("Using Vulkan debug message callback");
                instanceCreateInfo.pNext = nullptr;
            }

            uint32_t instanceLayerCount = 0;
            VKCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
            std::vector<VkLayerProperties> availableLayers(instanceLayerCount);
            VKCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, availableLayers.data()));

            bool allValidationLayersPresent = true;
            for (const char* layerName : m_InstanceLayers) {
                bool layerFound = false;
                for (const auto& layerProperties : availableLayers) {
                    if (strcmp(layerName, layerProperties.layerName) == 0) {
                        SNOW_CORE_INFO("Validation Layer {0} found", layerName);
                        layerFound = true;
                        break;
                    }
                }
                if (!layerFound) {
                    SNOW_CORE_WARN("Validation Layer {0} not found", layerName);
                    allValidationLayersPresent &= false;
                }
                else {
                    allValidationLayersPresent &= true;
                }
            }

            if (allValidationLayersPresent) {
                instanceCreateInfo.ppEnabledLayerNames = m_InstanceLayers.data();
                instanceCreateInfo.enabledLayerCount = m_InstanceLayers.size();
            }
            else {
                SNOW_CORE_WARN("Not all validation layers are present");
            }


            VKCheckError(vkCreateInstance(&instanceCreateInfo, nullptr, &s_VulkanInstance));
            
        }

        

        
    }
}