#pragma once

#include "Snow/Render/Material.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include "Snow/Platform/Vulkan/VulkanShader.h"

#include "Snow/Render/Image.h"
#include "Snow/Render/Texture.h"


namespace Snow {
	class VulkanMaterial : public Render::Material {
	public:
		VulkanMaterial(const Ref<Render::Shader>& shader, const std::string& name = "");
		VulkanMaterial(Ref<Render::Material> other, const std::string& name = "");
		virtual ~VulkanMaterial();

		virtual void Invalidate() override;

		virtual void Set(const std::string& name, float value) override;
		virtual void Set(const std::string& name, double value) override;
		virtual void Set(const std::string& name, int value) override;
		virtual void Set(const std::string& name, uint32_t value) override;
		virtual void Set(const std::string& name, bool value) override;
		virtual void Set(const std::string& name, const glm::vec2& value) override;
		virtual void Set(const std::string& name, const glm::vec3& value) override;
		virtual void Set(const std::string& name, const glm::vec4& value) override;
		virtual void Set(const std::string& name, const glm::mat3& value) override;
		virtual void Set(const std::string& name, const glm::mat4& value) override;

		virtual void Set(const std::string& name, const Ref<Render::Texture2D>& texture) override;
		virtual void Set(const std::string& name, const Ref<Render::Texture2D>& texture, uint32_t arrayIndex) override;
		virtual void Set(const std::string& name, const Ref<Render::TextureCube>& texture) override;
		virtual void Set(const std::string& name, const Ref<Render::Image2D>& image) override;

		virtual float& GetFloat(const std::string& name) override;
		virtual int32_t& GetInt(const std::string& name) override;
		virtual uint32_t& GetUInt(const std::string& name) override;
		virtual bool& GetBool(const std::string& name) override;
		virtual glm::vec2& GetVector2(const std::string& name) override;
		virtual glm::vec3& GetVector3(const std::string& name) override;
		virtual glm::vec4& GetVector4(const std::string& name) override;
		virtual glm::mat3& GetMatrix3(const std::string& name) override;
		virtual glm::mat4& GetMatrix4(const std::string& name) override;

		virtual Ref<Render::Texture2D> GetTexture2D(const std::string& name) override;
		virtual Ref<Render::TextureCube> GetTextureCube(const std::string& name) override;

		virtual Ref<Render::Texture2D> TryGetTexture2D(const std::string& name) override;
		virtual Ref<Render::TextureCube> TryGetTextureCube(const std::string& name) override;

		template<typename T>
		void Set(const std::string name, const T& value) {
			auto decl = FindUniformDeclaration(name);
			SNOW_CORE_ASSERT(decl, "Could not find uniform!");
			if (!decl)
				return;

			auto& buffer = m_UniformStorageBuffer;
			buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());
		}

		template<typename T>
		T& Get(const std::string name) {
			auto decl = FindUniformDeclaration(name);
			SNOW_CORE_ASSERT(decl, "Could not find uniform!");
			auto& buffer = m_UniformStorageBuffer;
			return buffer.Read<T>(decl->GetOffset());
		}

		template<typename T>
		Ref<T> GetResource(const std::string name) {
			auto decl = FindResourceDeclaration(name);
			SNOW_CORE_ASSERT(decl, "Could not find uniform!");
			uint32_t slot = decl->GetRegister();
			SNOW_CORE_ASSERT(slot < m_Textures.size(), "Texture slot is invalid!");
			return Ref<T>(m_Textures[slot]);
		}

		template<typename T>
		Ref<T> TryGetResource(const std::string name) {
			auto decl = FindResourceDeclaration(name);
			if (!decl)
				return nullptr;

			uint32_t slot = decl->GetRegister();
			if (slot >= m_Textures.size())
				return nullptr;

			return Ref<T>(m_Textures[slot]);
		}

		virtual uint32_t GetFlags() const override { return m_MaterialFlags; }
		virtual bool GetFlag(Render::MaterialFlag flag) const override { return (uint32_t)flag & m_MaterialFlags; }
		virtual void SetFlag(Render::MaterialFlag flag, bool value = true) override {
			if (value)
				m_MaterialFlags |= (uint32_t)flag;
			else
				m_MaterialFlags &= ~(uint32_t)flag;
		}

		virtual Ref<Render::Shader> GetShader() override { return m_Shader; }
		virtual const std::string& GetName() const override { return m_Name; }

		Render::VariableMap GetVariables() const override { return m_VariableMap; }
		virtual std::map<std::string, std::pair<Render::ResourceType, Ref<Render::Texture>>> GetTextures() const override { return m_MaterialTextures; }

		Buffer GetUniformStorageBuffer() { return m_UniformStorageBuffer; }

		void UpdateForRendering(const std::vector<std::vector<VkWriteDescriptorSet>>& uniformBufferWriteDescriptors = std::vector<std::vector<VkWriteDescriptorSet>>());
		void InvalidateDescriptorSets();

		const VkDescriptorSet GetDescriptorSet(uint32_t index) const { return !m_DescriptorSets[index].DescriptorSets.empty() ? m_DescriptorSets[index].DescriptorSets[0] : nullptr; }

	private:
		void Init();
		void AllocateStorage();
		void OnShaderReloaded();

		void SetVulkanDescriptor(const std::string& name, const Ref<Render::Texture2D>& texture);
		void SetVulkanDescriptor(const std::string& name, const Ref<Render::Texture2D>& texture, uint32_t arrayIndex);
		void SetVulkanDescriptor(const std::string& name, const Ref<Render::TextureCube>& texture);
		void SetVulkanDescriptor(const std::string& name, const Ref<Render::Image2D>& image);
		void SetVulkanDescriptor(const std::string& name, const VkDescriptorImageInfo& imageInfo);

		const Render::ShaderUniform* FindUniformDeclaration(const std::string& name);
		const Render::ShaderResource* FindResourceDeclaration(const std::string& name);

		enum class PendingDescriptorType {
			None = 0,
			Texture2D, TextureCube,
			Image2D
		};
		struct PendingDescriptor {
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WDS;
			VkDescriptorImageInfo ImageInfo{};
			Ref<Render::Texture> Texture;
			Ref<Render::Image> Image;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};
		struct PendingDescriptorArray {
			PendingDescriptorType Type = PendingDescriptorType::None;
			VkWriteDescriptorSet WDS;
			std::vector<VkDescriptorImageInfo> ImageInfos;
			std::vector<Ref<Render::Texture>> Textures;
			std::vector<Ref<Render::Image>> Images;
			VkDescriptorImageInfo SubmittedImageInfo{};
		};


		std::unordered_map<uint32_t, std::shared_ptr<PendingDescriptor>> m_ResidentDescriptors;
		std::unordered_map<uint32_t, std::shared_ptr<PendingDescriptorArray>> m_ResidentDescriptorArrays;
		std::vector<std::shared_ptr<PendingDescriptor>> m_PendingDescriptors;

		Ref<Render::Shader> m_Shader;
		std::string m_Name;

		uint32_t m_MaterialFlags = 0;

		Buffer m_UniformStorageBuffer;
		std::vector<Ref<Render::Texture>> m_Textures;
		std::vector<std::vector<Ref<Render::Texture>>> m_TextureArrays;
		std::vector<Ref<Render::Image>> m_Images;

		VulkanShader::ShaderMaterialDescriptorSet m_DescriptorSets[3];

		std::unordered_map<uint32_t, uint64_t> m_ImageHashes;

		std::vector<std::vector<VkWriteDescriptorSet>> m_WriteDescriptors;
		std::vector<bool> m_DirtyDescriptorSets;

		std::unordered_map<std::string, VkDescriptorImageInfo> m_ImageInfos;

		Render::VariableMap m_VariableMap;
		std::map<std::string, std::pair<Render::ResourceType, Ref<Render::Texture>>> m_MaterialTextures;
	};
}