#pragma once

#include "Snow/Render/Shader/Shader.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

#include <shaderc/shaderc.hpp>

namespace Snow {
	namespace Render {
		class VulkanShader : public Shader {
		public:
			VulkanShader(ShaderType type, const std::string& filePath);

			const ShaderType GetType() const override { return m_Type; }
			const std::string& GetPath() const override { return m_Path; }

			std::vector<uint32_t>& GetSPIRVBinaryData() { return m_SPIRVBinaryData; }

			VkShaderModule& GetVulkanShaderModule() { return m_VulkanShaderModule; }
		private:

			std::string ReadShaderFromFile(const std::string& path);
			
			shaderc_shader_kind SnowShaderTypeToShaderC(ShaderType type);
			void CreateSPIRVBinaryCache();
			void CreateVulkanShaderModule();

			VkShaderModule m_VulkanShaderModule;

			std::string m_Path;
			ShaderType m_Type;

			std::vector<uint32_t> m_SPIRVBinaryData;
			std::string m_Source;

		};
	}
}