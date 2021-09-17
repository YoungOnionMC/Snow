#include "spch.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Platform/Vulkan/VulkanImage.h"

#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#elif defined(SNOW_WINDOW_WIN32)
#include <Windows.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {

    static bool s_Validation = true;
    //VkInstance VulkanContext::s_VulkanInstance = nullptr;

    VulkanContext::VulkanContext(const Render::ContextSpecification& spec) :
        m_Specification(spec) {
    }

  //static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) {
        (void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix;
        SNOW_CORE_WARN("VulkanDebugCallback:\n Object Type: {0}\n Message: {1}", objectType, pMessage);

        const auto& imageRefs = VulkanImage2D::GetImageRefs();
        if (strstr(pMessage, "CoreValidation-DrawState-InvalidImageLayout"))
            SNOW_CORE_ASSERT(false);


        return VK_FALSE;
    }

    VulkanContext::~VulkanContext() {
        vkDestroyInstance(s_VulkanInstance, nullptr);
    }

    void VulkanContext::Create() {

        SNOW_CORE_INFO("Creating Vulkan Context");
#if defined(SNOW_WINDOW_GLFW)
        glfwMakeContextCurrent((GLFWwindow*)m_Specification.WindowHandle);
        if (!glfwVulkanSupported())
            SNOW_CORE_ERROR("GLFW does not support vulkan");
#endif

        // Application information 

        VkApplicationInfo applicationInfo = {};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = "Glacier";
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

        for (uint32_t i = 0; i < extensionCount; i++) {
            if (strcmp(extensions[i], VK_KHR_SURFACE_EXTENSION_NAME) == 0) {
                continue;
            }
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

        if (s_Validation) {
            m_InstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            m_InstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = NULL;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledExtensionCount = (uint32_t)m_InstanceExtensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = m_InstanceExtensions.data();
        

        bool debugUtils = false, debugReport = false;
        for (auto extension : m_InstanceExtensions) {
            if (extension == VK_EXT_DEBUG_UTILS_EXTENSION_NAME) {
                debugUtils = true;
            }
            if (extension == VK_EXT_DEBUG_REPORT_EXTENSION_NAME) {
                debugReport = true;
            }
        }

        std::vector<const char*> validationLayers;
        if (debugUtils && debugReport) {
            for (auto& validationLayer : m_InstanceLayers) {
                const char* validationLayerName = validationLayer;

                uint32_t instanceLayerCount = 0;
                VKCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr));
                std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
                VKCheckError(vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data()));
                bool validationLayerPresent = false;
                SNOW_CORE_TRACE("Vulkan Instance Layers:");
                for (const VkLayerProperties& layer : instanceLayerProperties) {
                    SNOW_CORE_TRACE("   {0}", layer.layerName);
                    if (strcmp(layer.layerName, validationLayerName) == 0) {
                        validationLayerPresent = true;
                        break;
                    }
                }
                if (validationLayerPresent) {
                    validationLayers.push_back(validationLayerName);
                }
                else {
                    SNOW_CORE_ERROR("Validation layer {0} not present, validation is disabled", validationLayer);
                }
            }


            instanceCreateInfo.enabledLayerCount = validationLayers.size();
            instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        }

        VKCheckError(vkCreateInstance(&instanceCreateInfo, nullptr, &s_VulkanInstance));

        if (debugUtils && debugReport) {
            auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(s_VulkanInstance, "vkCreateDebugReportCallbackEXT");
            SNOW_CORE_ASSERT(vkCreateDebugReportCallbackEXT != NULL, "Creation of vulkan debug extension callback failed");
            VkDebugReportCallbackCreateInfoEXT debugReportCI = {};
            debugReportCI.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
            debugReportCI.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            debugReportCI.pfnCallback = VulkanDebugReportCallback;
            debugReportCI.pUserData = NULL;
            VKCheckError(vkCreateDebugReportCallbackEXT(s_VulkanInstance, &debugReportCI, nullptr, &m_DebugReportCallback));
        }

        m_Device = Ref<VulkanDevice>::Create();

        VulkanAllocator::Init(m_Device);

    }

    void VulkanContext::BeginFrame() {
        m_SwapChain.BeginFrame();
    }

    void VulkanContext::SwapBuffers() {
        m_SwapChain.SwapBuffers();
    }


}