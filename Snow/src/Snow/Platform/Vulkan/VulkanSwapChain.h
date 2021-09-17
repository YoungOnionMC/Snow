#pragma once

#include "Snow/Render/SwapChain.h"


#if defined(SNOW_WINDOW_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(SNOW_WINDOW_GLFW)
	#if defined(SNOW_PLATFORM_WINDOWS)
        #define VK_USE_PLATFORM_WIN32_KHR
    #elif defined(SNOW_PLATFORM_LINUX)
        #define VK_USE_PLATFORM_XCB_KHR
    #endif
#endif
#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Platform/Vulkan/VulkanDevice.h"
#include "Snow/Platform/Vulkan/VulkanAllocator.h"

#include "Snow/Core/Ref.h"

namespace Snow {
	class VulkanSwapChain : public Render::SwapChain {
	public:
		VulkanSwapChain() = default;

		void Init() override;
		void Init(VkInstance instance, Ref<VulkanDevice>& device);

		virtual void InitSurface(void* windowHandle) override;

		void Create(const Render::SwapChainSpecification& spec);

		virtual void OnResize(uint32_t width, uint32_t height) override;
		virtual void BeginFrame() override;

		VkRenderPass GetRenderPass() { return m_RenderPass; }

		VkFramebuffer GetCurrentFramebuffer() { return GetFramebuffer(m_CurrentBufferIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetDrawCommandBuffer(m_CurrentBufferIndex); }

		uint32_t GetCurrentBufferIndex() override { return m_CurrentBufferIndex; }

		VkFramebuffer GetFramebuffer(uint32_t index) { SNOW_CORE_ASSERT(index < m_ImageCount); return m_Framebuffers[index]; }
		VkCommandBuffer GetDrawCommandBuffer(uint32_t index) { SNOW_CORE_ASSERT(index < m_ImageCount);  return m_DrawCommandBuffers[index]; }

		uint32_t GetImageCount() { return m_ImageCount; }
		uint32_t GetMinimumImageCount() { return m_MinimumImageCount; }

		uint32_t GetWidth() { return m_Width; }
		uint32_t GetHeight() { return m_Height; }

		void SwapBuffers() override;

	private:

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

		void CreateDrawBuffers();
		void FindImageFormatAndColorSpace();

		void CreateDepthStencil();
		void CreateFramebuffer();


		VkInstance m_Instance;
		Ref<VulkanDevice> m_Device;
		VulkanAllocator m_Allocator;

		VkSwapchainKHR m_VulkanSwapchain = nullptr;


		uint32_t m_QueueNodeIndex = UINT32_MAX;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

		uint32_t m_ImageCount = 0;
		uint32_t m_MinimumImageCount = 0;
		std::vector<VkImage> m_Images;

		struct SwapchainBuffer {
			VkImage Image;
			VkImageView View;
		};
		std::vector<SwapchainBuffer> m_Buffers;

		VkFormat m_DepthBufferFormat;
		struct DepthStencil {
			VkImage Image;
			VmaAllocation MemoryAlloc;
			VkImageView ImageView;
		} m_DepthStencil;

		std::vector<VkFramebuffer> m_Framebuffers;
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_DrawCommandBuffers;

		struct Semaphores {
			VkSemaphore PresentComplete;
			VkSemaphore RenderComplete;
		} m_Semaphores;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkFence> m_WaitFences;

		VkRenderPass m_RenderPass;
		uint32_t m_CurrentBufferIndex = 0;

		uint32_t m_Width = 0, m_Height = 0;
		bool m_VSync;

		Render::SwapChainSpecification m_Specification;

		VkSurfaceKHR m_VulkanSurface;

		VkResult m_Result;

	};

}