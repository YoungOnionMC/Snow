#include <spch.h>

#include "Snow/Platform/Vulkan/VulkanShader.h"

#include <filesystem>

#include <shaderc/shaderc.hpp>

#include "Snow/Platform/Vulkan/VulkanContext.h"

namespace Snow {

    std::string VulkanShader::ReadShaderFromFile(const std::string& path) {
        std::string result;
        std::ifstream inFile(path, std::ios::in | std::ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, std::ios::end);
            result.resize(inFile.tellg());
            inFile.seekg(0, std::ios::beg);
            inFile.read(&result[0], result.size());
        }
        else {
            SNOW_CORE_ERROR("Could not read file. (Path):{0}", path);
        }
        inFile.close();
        return result;
    }

    shaderc_shader_kind VulkanShader::SnowShaderTypeToShaderC(Render::ShaderType type) {
        switch (type) {
        case Render::ShaderType::Vertex:    return shaderc_shader_kind::shaderc_glsl_vertex_shader;
        case Render::ShaderType::Pixel:     return shaderc_shader_kind::shaderc_glsl_fragment_shader;
        }
    }

    VulkanShader::VulkanShader(Render::ShaderType type, const std::string& filePath) :
        m_Path(filePath), m_Type(type) {

        size_t found = filePath.find_last_of("/\\");
        m_Name = found != std::string::npos ? filePath.substr(found + 1) : filePath;
        found = m_Name.find_last_of(".");
        m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

        m_Source = ReadShaderFromFile(m_Path);
        CreateSPIRVBinaryCache();
        CreateVulkanShaderModule();
    }

    void VulkanShader::CreateSPIRVBinaryCache() {
        std::filesystem::path directoryPath = m_Path;
        if (!std::filesystem::is_directory(directoryPath.parent_path() / "cached")) {
            std::string dirPath = (directoryPath.parent_path() / "cached").string();
            std::filesystem::create_directory(directoryPath.parent_path() / "cached");
        }
        {
            std::filesystem::path filePath = m_Path;
            auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
            std::string cachedFilePath = path.string();

            FILE* f = fopen(cachedFilePath.c_str(), "rb");
            if (f) {
                fseek(f, 0, SEEK_END);
                uint64_t size = ftell(f);
                fseek(f, 0, SEEK_SET);
                m_SPIRVBinaryData = std::vector<uint32_t>(size / sizeof(uint32_t));
                fread(m_SPIRVBinaryData.data(), sizeof(uint32_t), m_SPIRVBinaryData.size(), f);
                fclose(f);
            }
        }

        if (m_SPIRVBinaryData.size() == 0) {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

            const bool optimize = true;
            if (optimize)
                options.SetOptimizationLevel(shaderc_optimization_level_performance);



            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(m_Source, SnowShaderTypeToShaderC(m_Type), m_Path.c_str(), options);

            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                SNOW_CORE_ERROR(module.GetErrorMessage());
            }

            m_SPIRVBinaryData = std::vector<uint32_t>(module.cbegin(), module.cend());

            {
                std::filesystem::path filePath = m_Path;
                auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                std::string cachedFilePath = path.string();

                FILE* f = fopen(cachedFilePath.c_str(), "wb");
                fwrite(m_SPIRVBinaryData.data(), sizeof(uint32_t), m_SPIRVBinaryData.size(), f);
                fclose(f);
            }
        }
    }

    void VulkanShader::CreateVulkanShaderModule() {
        Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();

        VkShaderModuleCreateInfo moduleCreateInfo = {};
        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.codeSize = m_SPIRVBinaryData.size() * sizeof(uint32_t);
        moduleCreateInfo.pCode = m_SPIRVBinaryData.data();

        vkCreateShaderModule(vkDevice->GetVulkanDevice(), &moduleCreateInfo, nullptr, &m_VulkanShaderModule);
    }
}