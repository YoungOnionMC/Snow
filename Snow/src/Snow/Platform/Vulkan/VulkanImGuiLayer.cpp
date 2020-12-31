#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

#include <imgui.h>

#define IMGUI_IMPL_API
#include <examples/imgui_impl_vulkan.h>
#include <examples/imgui_impl_vulkan.cpp>

#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
    #include <examples/imgui_impl_glfw.h>
    #include <examples/imgui_impl_glfw.cpp>
#elif defined(SNOW_WINDOW_WIN32)
    #include <windows.h>
    #include <examples/imgui_impl_win32.h>
    #include <examples/imgui_impl_win32.cpp>
#endif

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Core/Application.h"

namespace Snow {
    static void checkVKResult(VkResult error) {
        if(error == 0)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", error);
        if(error>0)
            abort();
    }

    VulkanImGuiLayer::VulkanImGuiLayer() {
        m_Name = "VulkanImGuiLayer";
    }

    void VulkanImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsClassic();
        ImGuiStyle& style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        Ref<Core::Window> window = Core::Application::Get().GetWindow();

        auto vkContext = VulkanContext::Get();
        auto vkDevice = vkContext->GetCurrentDevice();
        auto vkSwapChain = vkContext->GetSwapChain();

        VkDescriptorPool descriptorPool = {};

        VkDescriptorPoolSize poolSizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 },
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;
        auto result = vkCreateDescriptorPool(vkDevice->GetVulkanDevice(), &poolInfo, nullptr, &descriptorPool);
        checkVKResult(result);

#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Init((HWND)window->GetWindowHandle());
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window->GetWindowHandle(), true);
#endif
        ImGui_ImplVulkan_InitInfo info = {};
        info.Instance = VulkanContext::GetVulkanInstance();
        info.PhysicalDevice = vkDevice->GetPhysicalDevice();
        info.Device = vkDevice->GetVulkanDevice();
        info.QueueFamily = vkDevice->GetQueueFamilyIndices().Graphics;
        info.Queue = vkDevice->GetQueue();
        info.PipelineCache = nullptr;
        info.DescriptorPool = descriptorPool;
        info.Allocator = nullptr;
        info.MinImageCount = vkSwapChain.GetMinimumImageCount();
        info.ImageCount = vkSwapChain.GetImageCount();
        info.CheckVkResultFn = checkVKResult;
        ImGui_ImplVulkan_Init(&info, vkSwapChain.GetRenderPass());
    }

    void VulkanImGuiLayer::OnDetach() {
        auto vkContext = VulkanContext::Get();
        auto vkDevice = vkContext->GetDevice();

        VkResult error = vkDeviceWaitIdle(vkDevice->GetVulkanDevice());
        checkVKResult(error);
        ImGui_ImplVulkan_Shutdown();
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_Shutdown();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_Shutdown();
#endif
        ImGui::DestroyContext();
    }

    void VulkanImGuiLayer::BeginImGuiFrame() {
        ImGui_ImplVulkan_NewFrame();
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_NewFrame();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();
    }

    void VulkanImGuiLayer::EndImGuiFrame() {
        ImGuiIO& io = ImGui::GetIO();

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
}