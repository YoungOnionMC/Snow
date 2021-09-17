#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"

#include "Snow/Core/Application.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"

#if defined(SNOW_WINDOW_GLFW)
	#include <GLFW/glfw3.h>
#endif

#define GETINSTANCEPROCADDR(inst, entrypoint) \
	{										 \
		fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk"#entrypoint)); \
	}

#define GETDEVICEPROCADDR(dev, entrypoint) \
	{									   \
		fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk"#entrypoint));\
	}

static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
static PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
static PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
static PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
static PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
static PFN_vkQueuePresentKHR fpQueuePresentKHR;


namespace Snow {

	void VulkanSwapChain::Init() {
		auto vkContext = VulkanContext::Get();
		Init(vkContext->GetVulkanInstance(), vkContext->GetCurrentDevice());
	}

	void VulkanSwapChain::Init(VkInstance instance, Ref<VulkanDevice>& device) {
		m_Instance = instance;
		m_Device = device;

		GETDEVICEPROCADDR(m_Device->GetVulkanDevice(), CreateSwapchainKHR);
		GETDEVICEPROCADDR(m_Device->GetVulkanDevice(), DestroySwapchainKHR);
		GETDEVICEPROCADDR(m_Device->GetVulkanDevice(), GetSwapchainImagesKHR);
		GETDEVICEPROCADDR(m_Device->GetVulkanDevice(), AcquireNextImageKHR);
		GETDEVICEPROCADDR(m_Device->GetVulkanDevice(), QueuePresentKHR);

		GETINSTANCEPROCADDR(instance, GetPhysicalDeviceSurfaceSupportKHR);
		GETINSTANCEPROCADDR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GETINSTANCEPROCADDR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
		GETINSTANCEPROCADDR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
	}


	void VulkanSwapChain::InitSurface(void* windowHandle) {
		SNOW_CORE_INFO("Creating Vulkan surface");
#if defined(SNOW_WINDOW_WIN32)
		SNOW_CORE_INFO("Creating win32 window surface");
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = (HINSTANCE)GetWindowLong((HWND)windowHandle, GWLP_HINSTANCE);
		surfaceCreateInfo.hwnd = (HWND)windowHandle;
		VKCheckError(vkCreateWin32SurfaceKHR(m_Instance, &surfaceCreateInfo, NULL, &m_VulkanSurface));
#elif defined(SNOW_WINDOW_GLFW)
		SNOW_CORE_INFO("Creating GLFW window surface");
		VKCheckError(glfwCreateWindowSurface(m_Instance, (GLFWwindow*)windowHandle, nullptr, &m_VulkanSurface));
#endif

		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_Device->GetPhysicalDevice(), &queueCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueProperties(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_Device->GetPhysicalDevice(), &queueCount, queueProperties.data());

		std::vector<VkBool32> supportsPresent(queueCount);
		for (uint32_t i = 0; i < queueCount; i++) {
			fpGetPhysicalDeviceSurfaceSupportKHR(m_Device->GetPhysicalDevice(), i, m_VulkanSurface, &supportsPresent[i]);
		}

		uint32_t graphicsQueueNodeIndex = UINT32_MAX;
		uint32_t presentQueueNodeIndex = UINT32_MAX;
		for (uint32_t i = 0; i < queueCount; i++) {
			if ((queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
				if (graphicsQueueNodeIndex == UINT32_MAX)
					graphicsQueueNodeIndex = i;
				if (supportsPresent[i] == VK_TRUE) {
					graphicsQueueNodeIndex = i;
					presentQueueNodeIndex = i;
					break;
				}
			}
		}

		if (presentQueueNodeIndex == UINT32_MAX) {
			for (uint32_t i = 0; i < queueCount; i++) {
				if (supportsPresent[i] == VK_TRUE) {
					presentQueueNodeIndex = i;
					break;
				}
			}
		}

		m_QueueNodeIndex = graphicsQueueNodeIndex;
		FindImageFormatAndColorSpace();
	}

	void VulkanSwapChain::FindImageFormatAndColorSpace() {
		uint32_t formatCount;
		fpGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetPhysicalDevice(), m_VulkanSurface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		fpGetPhysicalDeviceSurfaceFormatsKHR(m_Device->GetPhysicalDevice(), m_VulkanSurface, &formatCount, surfaceFormats.data());
		if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)) {
			m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
			m_ColorSpace = surfaceFormats[0].colorSpace;
		}
		else {
			bool found_bgra8_unorm = false;
			for (auto&& surfaceFormat : surfaceFormats) {
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM) {
					m_ColorFormat = surfaceFormat.format;
					m_ColorSpace = surfaceFormat.colorSpace;
					found_bgra8_unorm = true;
					break;
				}
			}

			if (!found_bgra8_unorm) {
				m_ColorFormat = surfaceFormats[0].format;
				m_ColorSpace = surfaceFormats[0].colorSpace;
			}
		}
	}

	void VulkanSwapChain::Create(const Render::SwapChainSpecification& spec) {
		SNOW_CORE_INFO("Creating Vulkan SwapChain");
		m_Specification = spec;
		m_VSync = spec.VSync;

		VkSwapchainKHR oldSwapChain = m_VulkanSwapchain;
		VkSurfaceCapabilitiesKHR surfaceCaps;
		m_Result = fpGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Device->GetPhysicalDevice(), m_VulkanSurface, &surfaceCaps);

		uint32_t presentModeCount;
		m_Result = fpGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetPhysicalDevice(), m_VulkanSurface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		m_Result = fpGetPhysicalDeviceSurfacePresentModesKHR(m_Device->GetPhysicalDevice(), m_VulkanSurface, &presentModeCount, presentModes.data());

		VkExtent2D swapchainExtent = {};
		if (surfaceCaps.currentExtent.width == (uint32_t)-1) {
			swapchainExtent.width = spec.Width;
			swapchainExtent.height = spec.Height;
			m_Width = spec.Width;
			m_Height = spec.Height;
		}
		else {
			swapchainExtent = surfaceCaps.currentExtent;
			m_Width = surfaceCaps.currentExtent.width;
			m_Height = surfaceCaps.currentExtent.height;
		}
		

		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		if (!spec.VSync) {
			for (size_t i = 0; i < presentModeCount; i++) {
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if ((presentModes[i] != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
			}
		}

		uint32_t numSwapChainImages = surfaceCaps.minImageCount + 1;
		m_MinimumImageCount = surfaceCaps.minImageCount + 1;
		if ((surfaceCaps.maxImageCount > 0) && (numSwapChainImages > surfaceCaps.maxImageCount))
			numSwapChainImages = surfaceCaps.maxImageCount;

		VkSurfaceTransformFlagsKHR preTransform;
		if (surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		else
			preTransform = surfaceCaps.currentTransform;

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
		};

		for (auto& compositeAlphaFlag : compositeAlphaFlags) {
			if (surfaceCaps.supportedCompositeAlpha & compositeAlphaFlag) {
				compositeAlpha = compositeAlphaFlag;
				break;
			}
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.pNext = nullptr;
		swapchainCreateInfo.surface = m_VulkanSurface;
		swapchainCreateInfo.minImageCount = numSwapChainImages;
		swapchainCreateInfo.imageFormat = m_ColorFormat;
		swapchainCreateInfo.imageColorSpace = m_ColorSpace;
		swapchainCreateInfo.imageExtent = { swapchainExtent.width, swapchainExtent.height };
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		swapchainCreateInfo.presentMode = swapchainPresentMode;
		swapchainCreateInfo.oldSwapchain = oldSwapChain;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.compositeAlpha = compositeAlpha;

		if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
			swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		if (surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			swapchainCreateInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;


		VKCheckError(vkCreateSwapchainKHR(m_Device->GetVulkanDevice(), &swapchainCreateInfo, nullptr, &m_VulkanSwapchain));

		if (oldSwapChain != VK_NULL_HANDLE) {
			for (uint32_t i = 0; i < m_ImageCount; i++) {
				vkDestroyImageView(m_Device->GetVulkanDevice(), m_Buffers[i].View, nullptr);
			}
			fpDestroySwapchainKHR(m_Device->GetVulkanDevice(), oldSwapChain, nullptr);
		}
		m_Result = fpGetSwapchainImagesKHR(m_Device->GetVulkanDevice(), m_VulkanSwapchain, &m_ImageCount, nullptr);

		m_Images.resize(m_ImageCount);
		m_Result = fpGetSwapchainImagesKHR(m_Device->GetVulkanDevice(), m_VulkanSwapchain, &m_ImageCount, m_Images.data());

		m_Buffers.resize(m_ImageCount);
		for (uint32_t i = 0; i < m_ImageCount; i++) {
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.pNext = nullptr;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A,
			};
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			m_Buffers[i].Image = m_Images[i];
			colorAttachmentView.image = m_Buffers[i].Image;
			VKCheckError(vkCreateImageView(m_Device->GetVulkanDevice(), &colorAttachmentView, nullptr, &m_Buffers[i].View));
		}
		CreateDrawBuffers();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VKCheckError(vkCreateSemaphore(m_Device->GetVulkanDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.PresentComplete));
		VKCheckError(vkCreateSemaphore(m_Device->GetVulkanDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.RenderComplete));

		VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.pWaitDstStageMask = &pipelineStageFlags;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_WaitFences.resize(m_DrawCommandBuffers.size());
		for (auto& fence : m_WaitFences) {
			VKCheckError(vkCreateFence(m_Device->GetVulkanDevice(), &fenceCreateInfo, nullptr, &fence));
		}

		//CreateDepthStencil();

		m_DepthBufferFormat = VulkanContext::GetCurrentDevice()->GetDepthFormat();

		std::array<VkAttachmentDescription, 2> attachments = {};

		attachments[0].format = m_ColorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		attachments[1].format = m_DepthBufferFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDesc = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = 1;
		subpassDesc.pColorAttachments = &colorReference;
		//subpassDesc.pDepthStencilAttachment = &depthReference;
		subpassDesc.inputAttachmentCount = 0;
		subpassDesc.pInputAttachments = nullptr;
		subpassDesc.preserveAttachmentCount = 0;
		subpassDesc.pPreserveAttachments = nullptr;
		subpassDesc.pResolveAttachments = nullptr;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpassDesc;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &dependency;

		VKCheckError(vkCreateRenderPass(m_Device->GetVulkanDevice(), &renderPassCreateInfo, nullptr, &m_RenderPass));

		CreateFramebuffer();
	}

	void VulkanSwapChain::CreateDepthStencil() {
		

		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = m_DepthBufferFormat;
		imageCreateInfo.extent = { m_Width, m_Height, 1 };
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VulkanAllocator allocator("SwapChain");
		m_DepthStencil.MemoryAlloc = allocator.AllocateImage(imageCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_DepthStencil.Image);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.image = m_DepthStencil.Image;
		imageViewCreateInfo.format = m_DepthBufferFormat;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (m_DepthBufferFormat >= VK_FORMAT_D16_UNORM_S8_UINT)
			imageViewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VKCheckError(vkCreateImageView(m_Device->GetVulkanDevice(), &imageViewCreateInfo, nullptr, &m_DepthStencil.ImageView));
	}

	void VulkanSwapChain::CreateFramebuffer() {
		VkImageView ivAttachments[2];

		ivAttachments[1] = m_DepthStencil.ImageView;

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = ivAttachments;
		framebufferCreateInfo.width = m_Width;
		framebufferCreateInfo.height = m_Height;
		framebufferCreateInfo.layers = 1;

		m_Framebuffers.resize(m_ImageCount);
		for (uint32_t i = 0; i < m_Framebuffers.size(); i++) {
			ivAttachments[0] = m_Buffers[i].View;
			VKCheckError(vkCreateFramebuffer(m_Device->GetVulkanDevice(), &framebufferCreateInfo, nullptr, &m_Framebuffers[i]));
		}
	}

	void VulkanSwapChain::CreateDrawBuffers() {
		m_DrawCommandBuffers.resize(m_ImageCount);

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = m_QueueNodeIndex;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VKCheckError(vkCreateCommandPool(m_Device->GetVulkanDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocInfo = {};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.commandPool = m_CommandPool;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = static_cast<uint32_t>(m_DrawCommandBuffers.size());
		VKCheckError(vkAllocateCommandBuffers(m_Device->GetVulkanDevice(), &commandBufferAllocInfo, m_DrawCommandBuffers.data()));

	}

	void VulkanSwapChain::OnResize(uint32_t width, uint32_t height) {
		auto vkDevice = m_Device->GetVulkanDevice();
		VKCheckError(vkDeviceWaitIdle(vkDevice));

		Create(m_Specification);
		/*
		vkDestroyImageView(vkDevice, m_DepthStencil.ImageView, nullptr);
		VulkanAllocator allocator("SwapChain");
		allocator.DestroyImage(m_DepthStencil.Image, m_DepthStencil.MemoryAlloc);
		vkDestroyImage(vkDevice, m_DepthStencil.Image, nullptr);
		vkFreeMemory(vkDevice, m_DepthStencil.DeviceMemory, nullptr);
		*/
		//CreateDepthStencil();

		for (auto& framebuffer : m_Framebuffers)
			vkDestroyFramebuffer(m_Device->GetVulkanDevice(), framebuffer, nullptr);

		CreateFramebuffer();

		vkFreeCommandBuffers(m_Device->GetVulkanDevice(), m_CommandPool, static_cast<uint32_t>(m_DrawCommandBuffers.size()), m_DrawCommandBuffers.data());
		CreateDrawBuffers();

		VKCheckError(vkDeviceWaitIdle(m_Device->GetVulkanDevice()));

	}

	void VulkanSwapChain::BeginFrame() {
		AcquireNextImage(m_Semaphores.PresentComplete, &m_CurrentBufferIndex);
	}

	void VulkanSwapChain::SwapBuffers() {


		const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

		VKCheckError(vkWaitForFences(m_Device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX));
		

		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_DrawCommandBuffers[m_CurrentBufferIndex];

		VKCheckError(vkResetFences(m_Device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]));
		VKCheckError(vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]));

		VkResult result = QueuePresent(m_Device->GetGraphicsQueue(), m_CurrentBufferIndex, m_Semaphores.RenderComplete);

		//VKCheckError(result);

		if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
			if (result == VK_ERROR_OUT_OF_DATE_KHR) {
				OnResize(m_Width, m_Height);
				return;
			}
		}
		const auto& config = Render::Renderer::GetConfig();
		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % config.FramesInFlight;

		VKCheckError(vkWaitForFences(m_Device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, DEFAULT_FENCE_TIMEOUT));
	}

	VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex) {
		return fpAcquireNextImageKHR(m_Device->GetVulkanDevice(), m_VulkanSwapchain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
	}

	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore) {
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_VulkanSwapchain;
		presentInfo.pImageIndices = &imageIndex;
		if (waitSemaphore != VK_NULL_HANDLE) {
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &waitSemaphore;
		}
		return fpQueuePresentKHR(queue, &presentInfo);
	}
}