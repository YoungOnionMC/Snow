#include <spch.h>
#include "Snow/ImGui/ImGui.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/Vulkan/VulkanImage.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"

#include "examples/imgui_impl_vulkan_with_textures.h"

namespace Snow {
	namespace UI {



		void Image(const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {
				//Ref<OpenGLImage2D> glImage = image.As<OpenGLImage2D>();
				//ImGui::Image((ImTextureID)(size_t)glImage->GetRendererID(), size, uv0, uv1, tint_col, border_col);
			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanImage2D> vkImage = image.As<VulkanImage2D>();
				const auto& imageInfo = vkImage->GetImageInfo();
				if (!imageInfo.ImageView)
					return;
				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.Sampler, imageInfo.ImageView, vkImage->GetImageDescriptor().imageLayout);
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}

		void Image(const Ref<Render::Image2D>& image, uint32_t imageLayer, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {

			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanImage2D> vkImage = image.As<VulkanImage2D>();
				auto imageInfo = vkImage->GetImageInfo();
				imageInfo.ImageView = vkImage->GetLayerImageView(imageLayer);
				if (!imageInfo.ImageView)
					return;
				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.Sampler, imageInfo.ImageView, vkImage->GetImageDescriptor().imageLayout);
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}

		void ImageMip(const Ref<Render::Image2D>& image, uint32_t mip, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {

			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanImage2D> vkImage = image.As<VulkanImage2D>();
				auto imageInfo = vkImage->GetImageInfo();
				imageInfo.ImageView = vkImage->GetMipImageView(mip);
				if (!imageInfo.ImageView)
					return;
				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.Sampler, imageInfo.ImageView, vkImage->GetImageDescriptor().imageLayout);
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}

		void Image(const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {

			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanTexture2D> vkTexture = texture.As<VulkanTexture2D>();
				const auto& imageInfo = vkTexture->GetVulkanDescriptorInfo();
				if (!imageInfo.imageView)
					return;
				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}
	}
}