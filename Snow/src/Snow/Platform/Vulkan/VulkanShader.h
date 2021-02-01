#pragma once

#include "Snow/Render/Shader/Shader.h"
#include "Snow/Render/Shader/ShaderUniform.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include <shaderc/shaderc.hpp>

namespace Snow {
	class VulkanShader : public Render::Shader {
	public:
		VulkanShader(Render::ShaderType type, const std::string& filePath);

		void Reload() override {}

		//const Render::ShaderType GetType() const override { return m_Type; }
		const std::string& GetPath() const override { return m_Path; }
		const std::string& GetName() const override { return m_Name; }

		std::vector<uint32_t>& GetSPIRVBinaryData() { return m_SPIRVBinaryData; }

		VkShaderModule& GetVulkanShaderModule() { return m_VulkanShaderModule; }
	private:

		std::string ReadShaderFromFile(const std::string& path);

		shaderc_shader_kind SnowShaderTypeToShaderC(Render::ShaderType type);
		void CreateSPIRVBinaryCache();
		void CreateVulkanShaderModule();

		VkShaderModule m_VulkanShaderModule;

		std::string m_Path;
		std::string m_Name;
		Render::ShaderType m_Type;

		std::vector<uint32_t> m_SPIRVBinaryData;
		std::string m_Source;

	};
}