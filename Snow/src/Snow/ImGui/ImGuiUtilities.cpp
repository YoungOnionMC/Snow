#include <spch.h>

#include "Snow/ImGui/ImGuiUtilities.h"

#include "Snow/Render/RendererAPI.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"
#include <examples/imgui_impl_vulkan_with_textures.h>

namespace Snow {
	namespace UI {
		ImTextureID GetTextureID(Ref<Render::Texture2D> texture) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanTexture2D> vkTexture = texture.As<VulkanTexture2D>();

				//SNOW_CORE_VERIFY(vkTexture->GetImage());
				//if (!vkTexture->GetImage())
				//	return nullptr;

				const auto& imageInfo = vkTexture->GetVulkanDescriptorInfo();
				if (!imageInfo.imageView)
					return nullptr;

				return ImGui_ImplVulkan_AddTexture(imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);
			}
		}
	}
}