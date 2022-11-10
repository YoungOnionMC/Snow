#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanMaterial.h"

#include "Snow/Render/Renderer.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"
#include "Snow/Platform/Vulkan/VulkanImage.h"

#include "Snow/Utils/StringUtils.h"

#include <glm/gtc/type_ptr.hpp>

namespace Snow {
	VulkanMaterial::VulkanMaterial(const Ref<Render::Shader>& shader, const std::string& name) :
		m_Shader(shader), m_Name(name), m_WriteDescriptors(Render::Renderer::GetConfig().FramesInFlight),
		m_DirtyDescriptorSets(Render::Renderer::GetConfig().FramesInFlight) {

		Init();
		Render::Renderer::RegisterShaderDependency(shader, this);
	}

	VulkanMaterial::VulkanMaterial(Ref<Render::Material> other, const std::string& name) :
		m_Shader(other->GetShader()), m_Name(name),
		m_WriteDescriptors(Render::Renderer::GetConfig().FramesInFlight),
		m_DirtyDescriptorSets(Render::Renderer::GetConfig().FramesInFlight, true) {

		if (name.empty())
			m_Name = other->GetName();

		Render::Renderer::RegisterShaderDependency(m_Shader, this);

		auto vkMaterial = other.As<VulkanMaterial>();
		m_UniformStorageBuffer = Buffer::Copy(vkMaterial->m_UniformStorageBuffer.Data, vkMaterial->m_UniformStorageBuffer.Size);

		m_ResidentDescriptors = vkMaterial->m_ResidentDescriptors;
		m_ResidentDescriptorArrays = vkMaterial->m_ResidentDescriptorArrays;
		m_PendingDescriptors = vkMaterial->m_PendingDescriptors;
		m_Textures = vkMaterial->m_Textures;
		m_TextureArrays = vkMaterial->m_TextureArrays;
		m_Images = vkMaterial->m_Images;
		m_ImageHashes = vkMaterial->m_ImageHashes;

		m_VariableMap = vkMaterial->m_VariableMap;
		m_MaterialTextures = vkMaterial->m_MaterialTextures;
	}

	void VulkanMaterial::Init() {

		AllocateStorage();

		m_MaterialFlags |= (uint32_t)Render::MaterialFlag::DepthTest;
		m_MaterialFlags |= (uint32_t)Render::MaterialFlag::Blend;


#define InvalidateOnRT 1
#if InvalidateOnRT
		Ref<VulkanMaterial> instance = this;
		Render::Renderer::Submit([instance]() mutable {
			instance->Invalidate();
			});
#else
		Invalidate();
#endif

	}

	void VulkanMaterial::Invalidate() {
		uint32_t framesInFlight = Render::Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vkShader = m_Shader.As<VulkanShader>();

		if (vkShader->HasDescriptorSet(0)) {
			const auto& shaderDesciptorSets = vkShader->GetShaderDescriptorSet();
			if (!shaderDesciptorSets.empty()) {
				for (auto&& [binding, descriptor] : m_ResidentDescriptors) {
					
					m_PendingDescriptors.push_back(descriptor);
				}
			}
		}
	}

	VulkanMaterial::~VulkanMaterial() {}

	void VulkanMaterial::AllocateStorage() {
		const auto& shaderBuffers = m_Shader->GetShaderBuffers();

		if (shaderBuffers.size() > 0) {
			uint32_t size = 0;
			for (auto [name, shaderBuffer] : shaderBuffers) {
				size += shaderBuffer.Size;
				for (auto [uniformName, uniform] : shaderBuffer.Uniforms) {
					Buffer buffer(uniform.GetSize());
					buffer.ZeroInitialize();
					m_VariableMap[uniformName] = { uniform.GetType(), buffer };
				}
			}

			m_UniformStorageBuffer.Allocate(size);
			m_UniformStorageBuffer.ZeroInitialize();
		}

		const auto& shaderResources = m_Shader->GetResources();
		if (shaderResources.size()) {
			for (auto [name, resource] : shaderResources) {
				if (Snow::Utils::String::StartsWith(name, "u_Material")) {
					if (resource.GetType() == ResourceType::Texture2D) {
						m_MaterialTextures[name] = { ResourceType::Texture2D, Renderer::GetWhiteTexture() };
						Set(name, Renderer::GetWhiteTexture());
					}
					
				}
			}
		}
	}

	void VulkanMaterial::OnShaderReloaded() {
		return;

	}

	const Render::ShaderUniform* VulkanMaterial::FindUniformDeclaration(const std::string& name) {
		const auto& shaderBuffers = m_Shader->GetShaderBuffers();

		SNOW_CORE_ASSERT(shaderBuffers.size() <= 1, "Only support one material buffer rn");

		if (shaderBuffers.size() > 0) {
			const Render::ShaderBuffer& buffer = (*shaderBuffers.begin()).second;
			if (buffer.Uniforms.find(name) == buffer.Uniforms.end())
				return nullptr;

			return &buffer.Uniforms.at(name);
		}
		return nullptr;
	}

	const Render::ShaderResource* VulkanMaterial::FindResourceDeclaration(const std::string& name) {
		auto& resources = m_Shader->GetResources();
		for (const auto& [n, resource] : resources) {
			if (resource.GetName() == name)
				return &resource;
		}
		return nullptr;
	}

	void VulkanMaterial::SetVulkanDescriptor(const std::string& name, const Ref<Render::Texture2D>& texture) {
		const Render::ShaderResource* resource = FindResourceDeclaration(name);
		SNOW_CORE_ASSERT(resource);

		uint32_t binding = resource->GetRegister();
		if (binding < m_Textures.size() && m_Textures[binding] && texture->GetHash() == m_Textures[binding]->GetHash())
			return;

		if (binding >= m_Textures.size())
			m_Textures.resize(binding + 1);
		m_Textures[binding] = texture;

		const VkWriteDescriptorSet* wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		SNOW_CORE_ASSERT(wds);
		m_ResidentDescriptors[binding] = std::make_shared<PendingDescriptor>(PendingDescriptor{ PendingDescriptorType::Texture2D, *wds, {}, texture.As<Render::Texture>(), nullptr });
		m_PendingDescriptors.push_back(m_ResidentDescriptors.at(binding));
		InvalidateDescriptorSets();
	}

	void VulkanMaterial::SetVulkanDescriptor(const std::string& name, const Ref<Render::Texture2D>& texture, uint32_t arrayIndex) {
		const Render::ShaderResource* resource = FindResourceDeclaration(name);
		SNOW_CORE_ASSERT(resource);

		uint32_t binding = resource->GetRegister();
		if (binding < m_TextureArrays.size() && m_TextureArrays[binding].size() < arrayIndex && texture->GetHash() == m_TextureArrays[binding][arrayIndex]->GetHash())
			return;

		if (binding >= m_TextureArrays.size())
			m_TextureArrays.resize(binding + 1);

		if (arrayIndex >= m_TextureArrays[binding].size())
			m_TextureArrays[binding].resize(arrayIndex + 1);

		m_TextureArrays[binding][arrayIndex] = texture;

		const VkWriteDescriptorSet* wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name, resource->GetSet());
		SNOW_CORE_ASSERT(wds);
		if (m_ResidentDescriptorArrays.find(binding) == m_ResidentDescriptorArrays.end()) {
			m_ResidentDescriptorArrays[binding] = std::make_unique<PendingDescriptorArray>(PendingDescriptorArray{ PendingDescriptorType::Texture2D, *wds, {}, {}, {} });
		}

		auto& residentDescriptorArray = m_ResidentDescriptorArrays.at(binding);
		if (arrayIndex >= residentDescriptorArray->Textures.size())
			residentDescriptorArray->Textures.resize(arrayIndex + 1);

		residentDescriptorArray->Textures[arrayIndex] = texture;

		InvalidateDescriptorSets();
	}

	void VulkanMaterial::SetVulkanDescriptor(const std::string& name, const Ref<Render::TextureCube>& texture) {
		const Render::ShaderResource* resource = FindResourceDeclaration(name);
		SNOW_CORE_ASSERT(resource);

		uint32_t binding = resource->GetRegister();
		if (binding < m_Textures.size() && m_Textures[binding] && texture->GetHash() == m_Textures[binding]->GetHash())
			return;

		if (binding >= m_Textures.size())
			m_Textures.resize(binding + 1);
		m_Textures[binding] = texture;

		const VkWriteDescriptorSet* wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		SNOW_CORE_ASSERT(wds);
		m_ResidentDescriptors[binding] = std::make_shared<PendingDescriptor>(PendingDescriptor{ PendingDescriptorType::TextureCube, *wds, {}, texture.As<Render::Texture>(), nullptr });
		m_PendingDescriptors.push_back(m_ResidentDescriptors[binding]);
		InvalidateDescriptorSets();
	}

	void VulkanMaterial::SetVulkanDescriptor(const std::string& name, const Ref<Render::Image2D>& image) {
		const Render::ShaderResource* resource = FindResourceDeclaration(name);
		SNOW_CORE_ASSERT(resource);

		uint32_t binding = resource->GetRegister();
		if (binding < m_Images.size() && m_Images[binding] && m_ImageHashes.at(binding) == image->GetHash())
			return;

		if (binding >= m_Images.size())
			m_Images.resize(binding + 1);
		m_Images[binding] = image;
		m_ImageHashes[binding] = image->GetHash();

		const VkWriteDescriptorSet* wds = m_Shader.As<VulkanShader>()->GetDescriptorSet(name);
		SNOW_CORE_ASSERT(wds);
		m_ResidentDescriptors[binding] = std::make_shared<PendingDescriptor>(PendingDescriptor{ PendingDescriptorType::Image2D, *wds, {}, nullptr, image.As<Render::Image>() });
		m_PendingDescriptors.push_back(m_ResidentDescriptors[binding]);
		InvalidateDescriptorSets();
	}

	void VulkanMaterial::Set(const std::string& name, float value) {
		Set<float>(name, value);
		m_VariableMap.at(name).Data.Write(&value, sizeof(float));
	}

	void VulkanMaterial::Set(const std::string& name, double value) {
		Set<double>(name, value);
		m_VariableMap.at(name).Data.Write(&value, sizeof(double));
	}

	void VulkanMaterial::Set(const std::string& name, int value) {
		Set<int>(name, value);
		m_VariableMap.at(name).Data.Write(&value, sizeof(int));
	}

	void VulkanMaterial::Set(const std::string& name, uint32_t value) {
		Set<uint32_t>(name, value);
		m_VariableMap.at(name).Data.Write(&value, sizeof(uint32_t));
	}

	void VulkanMaterial::Set(const std::string& name, bool value) {
		Set<int>(name, (int)value);
		m_VariableMap.at(name).Data.Write(&value, sizeof(bool));
	}

	void VulkanMaterial::Set(const std::string& name, const glm::vec2& value) {
		Set<glm::vec2>(name, value);
		m_VariableMap.at(name).Data.Write((void*)glm::value_ptr(value), sizeof(glm::vec2));
	}

	void VulkanMaterial::Set(const std::string& name, const glm::vec3& value) {
		Set<glm::vec3>(name, value);
		m_VariableMap.at(name).Data.Write((void*)glm::value_ptr(value), sizeof(glm::vec3));
	}

	void VulkanMaterial::Set(const std::string& name, const glm::vec4& value) {
		Set<glm::vec4>(name, value);
		m_VariableMap.at(name).Data.Write((void*)glm::value_ptr(value), sizeof(glm::vec4));
	}

	void VulkanMaterial::Set(const std::string& name, const glm::mat3& value) {
		Set<glm::mat3>(name, value);
		m_VariableMap.at(name).Data.Write((void*)glm::value_ptr(value), sizeof(glm::mat3));
	}

	void VulkanMaterial::Set(const std::string& name, const glm::mat4& value) {
		Set<glm::mat4>(name, value);
		m_VariableMap.at(name).Data.Write((void*)glm::value_ptr(value), sizeof(glm::mat4));
	}

	void VulkanMaterial::Set(const std::string& name, const Ref<Render::Texture2D>& texture) {
		m_MaterialTextures[name] = { ResourceType::Texture2D, texture };
		SetVulkanDescriptor(name, texture);
	}

	void VulkanMaterial::Set(const std::string& name, const Ref<Render::Texture2D>& texture, uint32_t arrayIndex) {
		SetVulkanDescriptor(name, texture, arrayIndex);
	}

	void VulkanMaterial::Set(const std::string& name, const Ref<Render::TextureCube>& texture) {
		SetVulkanDescriptor(name, texture);
	}

	void VulkanMaterial::Set(const std::string& name, const Ref<Render::Image2D>& image) {
		SetVulkanDescriptor(name, image);
	}

	float& VulkanMaterial::GetFloat(const std::string& name) {
		return Get<float>(name);
	}

	int32_t& VulkanMaterial::GetInt(const std::string& name) {
		return Get<int32_t>(name);
	}

	uint32_t& VulkanMaterial::GetUInt(const std::string& name) {
		return Get<uint32_t>(name);
	}

	bool& VulkanMaterial::GetBool(const std::string& name) {
		return Get<bool>(name);
	}

	glm::vec2& VulkanMaterial::GetVector2(const std::string& name) {
		return Get<glm::vec2>(name);
	}

	glm::vec3& VulkanMaterial::GetVector3(const std::string& name) {
		return Get<glm::vec3>(name);
	}

	glm::vec4& VulkanMaterial::GetVector4(const std::string& name) {
		return Get<glm::vec4>(name);
	}

	glm::mat3& VulkanMaterial::GetMatrix3(const std::string& name) {
		return Get<glm::mat3>(name);
	}

	glm::mat4& VulkanMaterial::GetMatrix4(const std::string& name) {
		return Get<glm::mat4>(name);
	}

	Ref<Render::Texture2D> VulkanMaterial::GetTexture2D(const std::string& name) {
		return GetResource<Render::Texture2D>(name);
	}

	Ref<Render::TextureCube> VulkanMaterial::GetTextureCube(const std::string& name) {
		return GetResource<Render::TextureCube>(name);
	}

	Ref<Render::Texture2D> VulkanMaterial::TryGetTexture2D(const std::string& name) {
		return TryGetResource<Render::Texture2D>(name);
	}

	Ref<Render::TextureCube> VulkanMaterial::TryGetTextureCube(const std::string& name) {
		return TryGetResource<Render::TextureCube>(name);
	}

	void VulkanMaterial::UpdateForRendering(const std::vector<std::vector<VkWriteDescriptorSet>>& uniformBufferWriteDescriptors) {
		
		auto vkDevice = VulkanContext::GetCurrentDevice();

		for (auto&& [binding, descriptor] : m_ResidentDescriptors) {
			if (descriptor->Type == PendingDescriptorType::Image2D) {
				Ref<VulkanImage2D> image = descriptor->Image.As<VulkanImage2D>();
				SNOW_CORE_ASSERT(image->GetImageInfo().ImageView, "No ImageView");
				if (descriptor->WDS.pImageInfo && image->GetImageInfo().ImageView != descriptor->WDS.pImageInfo->imageView) {
					SNOW_CORE_WARN("Found out of date image2D descriptor ({0} vs {1})", (void*)image->GetImageInfo().ImageView, (void*)descriptor->WDS.pImageInfo->imageView);
					m_PendingDescriptors.emplace_back(descriptor);
					InvalidateDescriptorSets();
				}
			}
		}

		std::vector<VkDescriptorImageInfo> arrayImageInfos;

		uint32_t frameIndex = Render::Renderer::GetCurrentFrameIndex();

		if (m_DirtyDescriptorSets[frameIndex] || true) {
			m_DirtyDescriptorSets[frameIndex] = false;
			m_WriteDescriptors[frameIndex].clear();

			if (!uniformBufferWriteDescriptors.empty()) {
				for (auto& wd : uniformBufferWriteDescriptors[frameIndex])
					m_WriteDescriptors[frameIndex].push_back(wd);
			}
			for (auto&& [binding, pd] : m_ResidentDescriptors) {
				if (pd->Type == PendingDescriptorType::Texture2D) {
					Ref<VulkanTexture2D> texture = pd->Texture.As<VulkanTexture2D>();
					pd->ImageInfo = texture->GetVulkanDescriptorInfo();
					pd->WDS.pImageInfo = &pd->ImageInfo;
				}
				if (pd->Type == PendingDescriptorType::TextureCube) { 
					Ref<VulkanTextureCube> texture = pd->Texture.As<VulkanTextureCube>();
					pd->ImageInfo = texture->GetVulkanDescriptorInfo();
					pd->WDS.pImageInfo = &pd->ImageInfo;
				}
				if (pd->Type == PendingDescriptorType::Image2D) {
					Ref<VulkanImage2D> image = pd->Image.As<VulkanImage2D>();
					pd->ImageInfo = image->GetImageDescriptor();
					pd->WDS.pImageInfo = &pd->ImageInfo;
				}

				m_WriteDescriptors[frameIndex].push_back(pd->WDS);
			}

			for (auto&& [binding, pd] : m_ResidentDescriptorArrays) {
				if (pd->Type == PendingDescriptorType::Texture2D) {
					for (auto tex : pd->Textures) {
						Ref<VulkanTexture2D> texture = tex.As<VulkanTexture2D>();
						arrayImageInfos.emplace_back(texture->GetVulkanDescriptorInfo());
					}
				}
				pd->WDS.pImageInfo = arrayImageInfos.data();
				pd->WDS.descriptorCount = arrayImageInfos.size();
				m_WriteDescriptors[frameIndex].push_back(pd->WDS);
			}
		}

		auto vkShader = m_Shader.As<VulkanShader>();
		auto descriptorSet = vkShader->AllocateDescriptorSet();
		m_DescriptorSets[frameIndex] = descriptorSet;
		for (auto& writeDescriptor : m_WriteDescriptors[frameIndex]) {
			writeDescriptor.dstSet = descriptorSet.DescriptorSets[0];
		}

		vkUpdateDescriptorSets(vkDevice->GetVulkanDevice(), m_WriteDescriptors[frameIndex].size(), m_WriteDescriptors[frameIndex].data(), 0, nullptr);
		m_PendingDescriptors.clear();
	}

	void VulkanMaterial::InvalidateDescriptorSets() {
		const uint32_t frameInFlight = Render::Renderer::GetConfig().FramesInFlight;
		for (uint32_t i = 0; i < frameInFlight; i++) {
			m_DirtyDescriptorSets[i] = true;
		}
	}
}
