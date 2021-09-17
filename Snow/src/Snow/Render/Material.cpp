#include <spch.h>

#include "Snow/Render/Material.h"

#include "Snow/Platform/Vulkan/VulkanMaterial.h"

#include "Snow/Render/RendererAPI.h"

namespace Snow {
	namespace Render {

		Ref<Material> Material::Create(const Ref<Shader>& shader, const std::string& name) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanMaterial>::Create(shader, name);
			}
			return nullptr;
		}

		Ref<Material> Material::Copy(const Ref<Material>& other, const std::string& name) {
			switch (RendererAPI::Current()) {
			case RendererAPIType::None:	return nullptr;
			case RendererAPIType::Vulkan:	return Ref<VulkanMaterial>::Create(other, name);
			}
			return nullptr;
		}
	}
}