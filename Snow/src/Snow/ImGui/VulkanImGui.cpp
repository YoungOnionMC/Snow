#include <spch.h>
#include "Snow/ImGui/ImGui.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/Vulkan/VulkanImage.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"
#include "Snow/Platform/Vulkan/VulkanImGuiLayer.h"

#include "examples/imgui_impl_vulkan_with_textures.h"

namespace ImGui {
	extern bool ImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
}


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

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);

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

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);
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

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);
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

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);
				ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
			}
		}

		bool ImageButton(const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& background_col, const ImVec4& tint_col) {
			return ImageButton(nullptr, image, size, uv0, uv1, frame_padding, background_col, tint_col);
		}

		bool ImageButton(const char* stringID, const Ref<Render::Image2D>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& background_col, const ImVec4& tint_col) {
			if(Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {

			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanImage2D> vkImage = image.As<VulkanImage2D>();
				const auto& imageInfo = vkImage->GetImageInfo();

				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.Sampler, imageInfo.ImageView, vkImage->GetImageDescriptor().imageLayout);

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);
				ImGuiID id = (ImGuiID)((((uint64_t)imageInfo.ImageView) >> 32) ^ (uint32_t)imageInfo.ImageView);
				if (stringID) {
					const ImGuiID strID = ImGui::GetID(stringID);
					id = id ^ strID;
				}
				
				return ImGui::ImageButtonEx(id, textureID, size, uv0, uv1, ImVec2{ (float)frame_padding, (float)frame_padding }, background_col, tint_col);
			}
		}

		bool ImageButton(const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& background_col, const ImVec4& tint_col) {
			return ImageButton(nullptr, texture, size, uv0, uv1, frame_padding, background_col, tint_col);
		}

		bool ImageButton(const char* stringID, const Ref<Render::Texture2D>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& background_col, const ImVec4& tint_col) {
			if (Render::RendererAPI::Current() == Render::RendererAPIType::OpenGL) {

			}
			else if (Render::RendererAPI::Current() == Render::RendererAPIType::Vulkan) {
				Ref<VulkanTexture2D> vkTexture = texture.As<VulkanTexture2D>();

				SNOW_CORE_VERIFY(vkTexture->GetImage());
				if (!vkTexture->GetImage())
					return false;

				const auto& imageInfo = vkTexture->GetVulkanDescriptorInfo();

				const auto textureID = ImGui_ImplVulkan_AddTexture(imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);

				auto& vulkanImGuiLayer = Core::Application::Get().GetImGuiLayer().As<VulkanImGuiLayer>();
				vulkanImGuiLayer->GetTextureIDs().push_back((VkDescriptorSet)textureID);
				ImGuiID id = (ImGuiID)((((uint64_t)imageInfo.imageView) >> 32) ^ (uint32_t)imageInfo.imageView);
				if (stringID) {
					const ImGuiID strID = ImGui::GetID(stringID);
					id = id ^ strID;
				}

				return ImGui::ImageButtonEx(id, textureID, size, uv0, uv1, ImVec2{ (float)frame_padding, (float)frame_padding }, background_col, tint_col);
			}
		}
	}
}