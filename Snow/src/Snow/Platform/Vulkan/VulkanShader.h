#pragma once

#include "Snow/Render/Shader.h"
#include "Snow/Render/ShaderUniform.h"

#include "Snow/Core/Hash.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"
#include "vk_mem_alloc.h"

#include <shaderc/shaderc.hpp>

#include <glm/glm.hpp>

namespace Snow {

	

	

	

	struct VulkanImageDescriptor {
		VkDeviceMemory Memory;

		Render::ShaderResource Resource;
	};

	class VulkanShader : public Render::Shader {
	public:
		struct UniformBuffer {
			VkDescriptorBufferInfo Descriptor;
			uint32_t Size = 0;
			uint32_t BindingPoint = 0;
			std::string Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct StorageBuffer {
			VmaAllocation MemoryAlloc = nullptr;
			VkDescriptorBufferInfo Descriptor;
			uint32_t Size = 0;
			uint32_t BindingPoint = 0;
			std::string Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct ImageSampler {
			uint32_t BindingPoint = 0;
			uint32_t DescriptorSet = 0;
			uint32_t ArraySize;
			std::string Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct PushConstantRange {
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
			uint32_t Offset = 0;
			uint32_t Size = 0;
		};
	public:
		VulkanShader(const std::initializer_list<Render::ShaderModule>& shaderModules);

		void Reload(bool forceCompile = false) override;

		size_t GetHash() const override { return Hash::GenerateFNVHash(m_AssetPath); }
		//void Bind() const override {}

		//const Render::ShaderType GetType() const override { return m_Type; }
		const std::string& GetPath() const override { return m_Paths[0]; }
		const std::string& GetName() const override { return m_Name; }

		virtual const uint32_t GetModuleCount() const override { return (uint32_t)m_VulkanShaderModules.size(); }

		virtual const std::unordered_map<std::string, Render::ShaderBuffer>& GetShaderBuffers() const override { return m_Buffers; }
		virtual const std::unordered_map<std::string, Render::ShaderResource>& GetResources() const override { return m_Resources; }

		virtual std::vector<Render::ShaderType> GetModuleTypes() { return m_ShaderTypes; }

		//virtual void SetUniformBufferData(const std::string& uniformBufferName, void* data = nullptr, uint32_t size = 0) override {}
		//virtual void SetUniformBufferData(uint32_t bindingPoint, void* data = nullptr, uint32_t size = 0) override {}

		

		VkDescriptorSet GetDescriptorSet() { return m_DescriptorSet; }
		VkDescriptorSetLayout GetDescriptorSetLayout(uint32_t set) { return m_DescriptorSetLayouts.at(set); }
		std::vector<VkDescriptorSetLayout> GetAllDescriptorSetLayouts();

		UniformBuffer& GetUniformBuffer(uint32_t binding = 0, uint32_t set = 0) { SNOW_CORE_ASSERT(m_ShaderDescriptorSets.at(set).UniformBuffers.size() > binding); return *m_ShaderDescriptorSets.at(set).UniformBuffers[binding]; }
		uint32_t GetUniformBufferCount(uint32_t set = 0) {
			if (m_ShaderDescriptorSets.size() < set)
				return 0;

			return (uint32_t)m_ShaderDescriptorSets[set].UniformBuffers.size();
		}

		const std::vector<PushConstantRange>& GetPushConstantRanges() const { return m_PushConstantRanges; }

		struct ShaderDescriptorSet {
			std::unordered_map<uint32_t, UniformBuffer*> UniformBuffers;
			std::unordered_map<uint32_t, StorageBuffer*> StorageBuffers;
			std::unordered_map<uint32_t, ImageSampler> ImageSamplers;
			std::unordered_map<uint32_t, ImageSampler> StorageImages;

			std::unordered_map<std::string, VkWriteDescriptorSet> WriteDescriptorSets;

			operator bool() const { return !(StorageBuffers.empty() && UniformBuffers.empty() && ImageSamplers.empty() && StorageImages.empty()); }
		};
		const std::vector<ShaderDescriptorSet>& GetShaderDescriptorSet() const { return m_ShaderDescriptorSets; }
		bool HasDescriptorSet(uint32_t set) const { return m_TypeCounts.find(set) != m_TypeCounts.end(); }
		
		struct ShaderMaterialDescriptorSet {
			VkDescriptorPool Pool;
			std::vector<VkDescriptorSet> DescriptorSets;
		};

		ShaderMaterialDescriptorSet AllocateDescriptorSet(uint32_t set = 0);
		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set = 0);
		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set, uint32_t numberOffsets);
		const VkWriteDescriptorSet* GetDescriptorSet(const std::string& name, uint32_t set = 0) const;

		static void ClearUniformBuffers();

		std::vector<VkVertexInputAttributeDescription> GetVertexInputAttributes() { return m_VertexInputAttributeDescriptions; }

		Render::ShaderType GetShaderModuleType(uint32_t index) const { return m_ShaderSourceModules[index].first; }
		VkShaderModule& GetVulkanShaderModule(uint32_t shaderIndex) { return m_VulkanShaderModules[shaderIndex]; }

	private:

		std::string ReadShaderFromFile(const std::string& path);
		void CreateShaderModuleStrings();
		void Load();

		std::vector<Render::ShaderModule> PreProcess(const std::string& source);

		shaderc_shader_kind SnowShaderTypeToShaderC(Render::ShaderType type);
		void CreateSPIRVBinaryCache(uint32_t shaderIndex);
		void CreateVulkanShaderModule(uint32_t shaderIndex);
		void Reflect(std::vector<uint32_t>& data, Render::ShaderType type);

		void CreateDescriptors();
		//VkDescriptorSet CreateDescriptorSet();

		bool m_ForceRecompile;
		std::vector<VkShaderModule> m_VulkanShaderModules;

		std::vector<Render::ShaderModule> m_ShaderPathModules;
		std::vector<Render::ShaderModule> m_ShaderSourceModules;
		std::vector<Render::ShaderType> m_ShaderTypes;

		std::string m_AssetPath;
		std::vector<std::string> m_Paths;
		std::string m_Name;

		std::vector<ShaderDescriptorSet> m_ShaderDescriptorSets;

		std::vector<PushConstantRange> m_PushConstantRanges;
		std::unordered_map<std::string, Render::ShaderResource> m_Resources;
		std::unordered_map<std::string, Render::ShaderBuffer> m_Buffers;

		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		VkDescriptorSet m_DescriptorSet;
		VkDescriptorPool m_DescriptorPool;

		std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> m_TypeCounts;

		std::vector<std::vector<uint32_t>> m_SPIRVBinaryData;

		uint32_t m_Hash;


		std::vector<VkVertexInputAttributeDescription> m_VertexInputAttributeDescriptions;
	};
}