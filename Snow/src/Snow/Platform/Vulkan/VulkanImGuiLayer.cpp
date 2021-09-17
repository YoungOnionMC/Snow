#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

#include <imgui.h>

#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
//#include <examples/imgui_impl_vulkan.h>
//#include <examples/imgui_impl_vulkan.cpp>

#include <examples/imgui_impl_vulkan_with_textures.h>
//#include <examples/imgui_impl_vulkan_with_textures.cpp>

#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
    #include <examples/imgui_impl_glfw.h>
    #include <examples/imgui_impl_glfw.cpp>
#elif defined(SNOW_WINDOW_WIN32)
    #include <windows.h>
    #include <examples/imgui_impl_win32.h>
    #include <examples/imgui_impl_win32.cpp>
#endif

#include <ImGuizmo.h>

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Core/Application.h"

namespace Snow {

    static std::vector<VkCommandBuffer> s_ImGuiCommandBuffer;
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

        ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        io.FontDefault = io.Fonts->Fonts.back();

        ImGui::StyleColorsClassic();
        ImGuiStyle& style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        Ref<Core::Window> window = Core::Application::Get().GetWindow();

        auto vkContext = VulkanContext::Get();
        auto vkDevice = vkContext->GetCurrentDevice();
        auto vkSwapChain = window->GetSwapChain().As<VulkanSwapChain>();

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
        info.Queue = vkDevice->GetGraphicsQueue();
        info.PipelineCache = nullptr;
        info.DescriptorPool = descriptorPool;
        info.Allocator = nullptr;
        info.MinImageCount = vkSwapChain->GetMinimumImageCount();
        info.ImageCount = vkSwapChain->GetImageCount();
        info.CheckVkResultFn = checkVKResult;
        ImGui_ImplVulkan_Init(&info, vkSwapChain->GetRenderPass());

        {
            VkCommandBuffer commandBuffer = vkDevice->GetCommandBuffer(true);
            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
            vkDevice->FlushCommandBuffer(commandBuffer);

            result = vkDeviceWaitIdle(vkDevice->GetVulkanDevice());
            checkVKResult(result);
            ImGui_ImplVulkan_DestroyFontUploadObjects();

        }

        uint32_t framesInFlight = Render::Renderer::GetConfig().FramesInFlight;
        s_ImGuiCommandBuffer.resize(framesInFlight);
        for(uint32_t i=0; i< framesInFlight; i++)
            s_ImGuiCommandBuffer[i] = vkDevice->CreateSecondaryCommandBuffer();
    }

    void VulkanImGuiLayer::OnDetach() {
        Render::Renderer::Submit([]() {
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
        });
    }

    void VulkanImGuiLayer::BeginImGuiFrame() {
        ImGui_ImplVulkan_NewFrame();
#if defined(SNOW_WINDOW_WIN32)
        ImGui_ImplWin32_NewFrame();
#elif defined(SNOW_WINDOW_GLFW)
        ImGui_ImplGlfw_NewFrame();
#endif
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void VulkanImGuiLayer::EndImGuiFrame() {
        ImGui::Render();

        Ref<VulkanContext> context = VulkanContext::Get();
        auto swapChain = Core::Application::Get().GetWindow()->GetSwapChain().As<VulkanSwapChain>();
        VkCommandBuffer drawCommandBuffer = swapChain->GetCurrentDrawCommandBuffer();
        VkCommandBufferBeginInfo drawCommandBufferInfo = {};
        drawCommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        drawCommandBufferInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        drawCommandBufferInfo.pNext = nullptr;

        
        vkBeginCommandBuffer(drawCommandBuffer, &drawCommandBufferInfo);

        uint32_t commandBufferIndex = swapChain->GetCurrentBufferIndex();

        VkClearValue clearValues[2];
        clearValues[0].color = { {1.1f, 0.1f ,0.1f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        uint32_t width = swapChain->GetWidth();
        uint32_t height = swapChain->GetHeight();

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.pNext = nullptr;
        renderPassInfo.renderPass = swapChain->GetRenderPass();
        renderPassInfo.renderArea.offset.x = 0;
        renderPassInfo.renderArea.offset.y = 0;
        renderPassInfo.renderArea.extent.width = swapChain->GetWidth();
        renderPassInfo.renderArea.extent.height = swapChain->GetHeight();
        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues;
        renderPassInfo.framebuffer = swapChain->GetCurrentFramebuffer();

        vkCmdBeginRenderPass(drawCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

        VkCommandBufferInheritanceInfo inheritanceInfo = {};
        inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritanceInfo.renderPass = swapChain->GetRenderPass();
        inheritanceInfo.framebuffer = swapChain->GetCurrentFramebuffer();

        VkCommandBufferBeginInfo cmdBufInfo = {};
        cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

        VKCheckError(vkBeginCommandBuffer(s_ImGuiCommandBuffer[commandBufferIndex], &cmdBufInfo));

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = (float)height;
        viewport.height = -(float)height;
        viewport.width = (float)width;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(s_ImGuiCommandBuffer[commandBufferIndex], 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.extent.width = width;
        scissor.extent.height = height;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(s_ImGuiCommandBuffer[commandBufferIndex], 0, 1, &scissor);

        ImDrawData* mainDrawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(mainDrawData, s_ImGuiCommandBuffer[commandBufferIndex]);

        VKCheckError(vkEndCommandBuffer(s_ImGuiCommandBuffer[commandBufferIndex]));

        std::vector<VkCommandBuffer> commandBuffers;
        commandBuffers.push_back(s_ImGuiCommandBuffer[commandBufferIndex]);

        vkCmdExecuteCommands(drawCommandBuffer, commandBuffers.size(), commandBuffers.data());

        vkCmdEndRenderPass(drawCommandBuffer);

        VKCheckError(vkEndCommandBuffer(drawCommandBuffer));


        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
}