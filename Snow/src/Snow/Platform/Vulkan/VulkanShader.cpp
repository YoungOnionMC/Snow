#include <spch.h>

#include "Snow/Platform/Vulkan/VulkanShader.h"

#include "Snow/Core/Hash.h"

#include <filesystem>

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanRenderer.h"

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
        case Render::ShaderType::Geometry:     return shaderc_shader_kind::shaderc_glsl_geometry_shader;
        //case Render::ShaderType::Tessalation:     return shaderc_shader_kind::tess;
        case Render::ShaderType::Compute:   return shaderc_shader_kind::shaderc_compute_shader;
        }
    }

    static std::unordered_map<uint32_t, std::unordered_map<uint32_t, VulkanShader::UniformBuffer*>> s_UniformBuffers; // set->binding
    static std::unordered_map<uint32_t, std::unordered_map<uint32_t, VulkanShader::StorageBuffer*>> s_StorageBuffers; // set->binding

    VulkanShader::VulkanShader(const std::initializer_list<Render::ShaderModule>& shaderModules) :
        m_ShaderPathModules(shaderModules) {

        for (auto [type, path] : m_ShaderPathModules) {
            m_Paths.push_back(path);
            size_t found = path.find_last_of("/\\");
            m_Name = found != std::string::npos ? path.substr(found + 1) : path;
            found = m_Name.find_first_of(".");
            m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

        }

        bool forceCompile = true;
        Reload(forceCompile);

        
    }

    void VulkanShader::ClearUniformBuffers() {
        s_UniformBuffers.clear();
        s_StorageBuffers.clear();
    }

    void VulkanShader::CreateShaderModuleStrings() {
        if (m_ShaderPathModules.size() > 1) {
            m_ShaderSourceModules.resize(m_ShaderPathModules.size());
            for (uint32_t i = 0; i < m_ShaderSourceModules.size(); i++) {
                //auto& [type, path] : m_ShaderModules
                m_ShaderSourceModules[i].second = ReadShaderFromFile(m_ShaderPathModules[i].second);
                m_ShaderSourceModules[i].first = m_ShaderPathModules[i].first;
                SNOW_CORE_ASSERT(m_ShaderSourceModules[i].first != Render::ShaderType::None);

            }
        }
        else if (m_ShaderPathModules.size() == 1) {
            SNOW_CORE_ASSERT(m_ShaderPathModules[0].second != "");
            const auto& source = ReadShaderFromFile(m_ShaderPathModules[0].second);
            auto shaderModules = PreProcess(source);
            m_ShaderSourceModules = shaderModules;

            std::string originalPath = m_Paths[0];
            m_Paths.resize(m_ShaderSourceModules.size());
            size_t found = originalPath.find_last_of("/\\");
            std::string parentPath = found != std::string::npos ? originalPath.substr(0, found + 1) : originalPath;
            for (uint32_t i = 0; i < m_ShaderSourceModules.size(); i++)
                m_Paths[i] = std::string(parentPath + m_Name + "." + ShaderTypeToString(m_ShaderSourceModules[i].first) + ".glsl");

        }
        else {
            SNOW_CORE_ERROR("Shader failed to reload!");
        }
    }

    void VulkanShader::Reload(bool forceCompile) {

        Ref<VulkanShader> instance = this;
        Render::Renderer::Submit([instance, forceCompile]() mutable {
            instance->m_ShaderDescriptorSets.clear();
            instance->m_Resources.clear();
            instance->m_Buffers.clear();
            instance->m_PushConstantRanges.clear();
            instance->m_DescriptorSetLayouts.clear();
            instance->m_TypeCounts.clear();
            instance->m_ShaderSourceModules.clear();

            instance->CreateShaderModuleStrings();

            instance->m_Hash = Hash::GenerateFNVHash(instance->m_ShaderSourceModules[0].second);

            instance->Load();

            Render::Renderer::OnShaderReloaded(instance->GetHash());
        });
    }

    void VulkanShader::Load() {
        m_SPIRVBinaryData.resize(m_ShaderSourceModules.size());
        m_VulkanShaderModules.resize(m_ShaderSourceModules.size());
        for (uint32_t i = 0; i < m_SPIRVBinaryData.size(); i++) {
            CreateSPIRVBinaryCache(i);
            CreateVulkanShaderModule(i);
        }

        for (uint32_t i = 0; i < m_ShaderSourceModules.size(); i++) {
            Reflect(m_SPIRVBinaryData[i], m_ShaderSourceModules[i].first);
        }


        CreateDescriptors();
        
    }

    static Render::ShaderType ShaderTypeFromString(const std::string& type) {
        if (type == "vertex")
            return Render::ShaderType::Vertex;
        if (type == "fragment" || type == "pixel")
            return Render::ShaderType::Pixel;
        if (type == "compute")
            return Render::ShaderType::Compute;
    }

    static std::string ShaderTypeToString(Render::ShaderType type) {
        switch (type) {
        case Render::ShaderType::Vertex:    return "vertex";
        case Render::ShaderType::Pixel:    return "pixel";
        case Render::ShaderType::Compute:    return "compute";
        case Render::ShaderType::Geometry:    return "geometry";
        }
    }

    std::vector<std::pair<Render::ShaderType, std::string>> VulkanShader::PreProcess(const std::string& source) {
        std::vector<std::pair<Render::ShaderType, std::string>> shaderMap;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            SNOW_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            SNOW_CORE_ASSERT(type == "vertex" || type == "pixel" || type == "fragment" || type == "compute" || type == "geometry", "Invalid shader type");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            Render::ShaderType shaderType = ShaderTypeFromString(type);

            shaderMap.push_back({ shaderType, source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)) });
        }

        return shaderMap;
    }

    void VulkanShader::CreateSPIRVBinaryCache(uint32_t shaderIndex) {
        {
            std::filesystem::path directoryPath = m_Paths[shaderIndex];
            if (!std::filesystem::is_directory(directoryPath.parent_path() / "cached")) {
                std::string dirPath = (directoryPath.parent_path() / "cached").string();
                std::filesystem::create_directory(directoryPath.parent_path() / "cached");
            }
            {
                std::filesystem::path filePath = m_Paths[shaderIndex];
                auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                std::string cachedFilePath = path.string();

                FILE* f = fopen(cachedFilePath.c_str(), "rb");
                if (f) {
                    fseek(f, 0, SEEK_END);
                    uint64_t size = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    m_SPIRVBinaryData[shaderIndex] = std::vector<uint32_t>(size / sizeof(uint32_t));
                    fread(m_SPIRVBinaryData[shaderIndex].data(), sizeof(uint32_t), m_SPIRVBinaryData[shaderIndex].size(), f);
                    fclose(f);
                }
            }
        }

        if (m_SPIRVBinaryData[shaderIndex].size() == 0) {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
            options.SetOptimizationLevel(shaderc_optimization_level_zero);

            const bool optimize = false;
            if (optimize)
                options.SetOptimizationLevel(shaderc_optimization_level_performance);

            printf("--------------\n");
            printf("Shader: %s\n", m_ShaderSourceModules[shaderIndex].second.c_str());
            printf("--------------\n");


            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(m_ShaderSourceModules[shaderIndex].second, SnowShaderTypeToShaderC(m_ShaderSourceModules[shaderIndex].first), m_Paths[shaderIndex].c_str(), options);

            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                SNOW_CORE_ERROR(module.GetErrorMessage());
                SNOW_CORE_ASSERT(false);
            }

            m_SPIRVBinaryData[shaderIndex] = std::vector<uint32_t>(module.cbegin(), module.cend());

            SNOW_CORE_TRACE("Created SPIRV module for OpenGL Shader {0}", m_Paths[shaderIndex]);
            SNOW_CORE_TRACE("   size in bytes: {0}", m_SPIRVBinaryData[shaderIndex].size());

            {
                std::filesystem::path filePath = m_Paths[shaderIndex];
                auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                std::string cachedFilePath = path.string();

                FILE* f = fopen(cachedFilePath.c_str(), "wb");
                fwrite(m_SPIRVBinaryData[shaderIndex].data(), sizeof(uint32_t), m_SPIRVBinaryData[shaderIndex].size(), f);
                fclose(f);
            }
        }
    }

    void VulkanShader::CreateVulkanShaderModule(uint32_t shaderIndex) {
        Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();

        VkShaderModuleCreateInfo moduleCreateInfo = {};
        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.codeSize = m_SPIRVBinaryData[shaderIndex].size() * sizeof(uint32_t);
        moduleCreateInfo.pCode = m_SPIRVBinaryData[shaderIndex].data();

        vkCreateShaderModule(vkDevice->GetVulkanDevice(), &moduleCreateInfo, nullptr, &m_VulkanShaderModules[shaderIndex]);
    }

    static Render::UniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type) {
        using namespace spirv_cross;
        using namespace Render;

        UniformType retType = UniformType::None;
        switch (type.basetype) {
        case spirv_cross::SPIRType::Boolean:    retType = UniformType::Bool; break;
        case spirv_cross::SPIRType::Int: {
            if (type.vecsize == 1)   retType = UniformType::Int;
            else if (type.vecsize == 2)   retType = UniformType::Int2;
            else if (type.vecsize == 3)   retType = UniformType::Int3;
            else if (type.vecsize == 4)   retType = UniformType::Int4;
            break;
        }
        case spirv_cross::SPIRType::Float: {
            if (type.vecsize == 1)   retType = UniformType::Float;
            else if (type.vecsize == 2)   retType = UniformType::Float2;
            else if (type.vecsize == 3) {
                retType = UniformType::Float3;


                if (type.columns == 3)  retType = UniformType::Mat3x3;
            }
            else if (type.vecsize == 4) {
                retType = UniformType::Float4;

                if (type.columns == 4)  retType = UniformType::Mat4x4;
            }
            break;
        }
        case spirv_cross::SPIRType::Struct:
            retType = UniformType::Struct;
            break;
        }
        return retType;
    }

    static VkShaderStageFlagBits ShaderTypeToVkShaderStage(Render::ShaderType type) {
        switch (type) {
        case Render::ShaderType::None:  return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
        case Render::ShaderType::Vertex:    return VK_SHADER_STAGE_VERTEX_BIT;
        case Render::ShaderType::Pixel:    return VK_SHADER_STAGE_FRAGMENT_BIT;
        case Render::ShaderType::Compute:    return VK_SHADER_STAGE_COMPUTE_BIT;
        case Render::ShaderType::Geometry:    return VK_SHADER_STAGE_GEOMETRY_BIT;
        }
        SNOW_CORE_ERROR("Shader stage not present");
        return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }

    static VkFormat SPIRTypeToVKFormat(spirv_cross::SPIRType type) {
        switch (type.basetype) {
        case spirv_cross::SPIRType::Boolean:	return VK_FORMAT_R16_SINT;
        case spirv_cross::SPIRType::Int: {
            if (type.vecsize == 1)	return VK_FORMAT_R32_UINT;
            if (type.vecsize == 2)	return VK_FORMAT_R32G32_UINT;
            if (type.vecsize == 3)	return VK_FORMAT_R32G32B32_UINT;
            if (type.vecsize == 4)	return VK_FORMAT_R32G32B32A32_UINT;
        }
        case spirv_cross::SPIRType::Float: {
            if (type.vecsize == 1)	return VK_FORMAT_R32_SFLOAT;
            if (type.vecsize == 2)	return VK_FORMAT_R32G32_SFLOAT;
            if (type.vecsize == 3)	return VK_FORMAT_R32G32B32_SFLOAT;
            if (type.vecsize == 4)	return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        }
    }

    static uint32_t GetSizeFromSPIRType(spirv_cross::SPIRType type) {
        switch (type.basetype) {
        case spirv_cross::SPIRType::Int: {
            if (type.vecsize == 1)   return sizeof(int);
            if (type.vecsize == 2)   return sizeof(int) * 2;
            if (type.vecsize == 3)   return sizeof(int) * 3;
            if (type.vecsize == 4)   return sizeof(int) * 4;
        }
        case spirv_cross::SPIRType::Float: {
            if (type.vecsize == 1)   return sizeof(float);
            if (type.vecsize == 2)   return sizeof(float) * 2;
            if (type.vecsize == 3)   return sizeof(float) * 3;
            if (type.vecsize == 4)   return sizeof(float) * 4;
        }
        }
    }

    void VulkanShader::Reflect(std::vector<uint32_t>& data, Render::ShaderType shaderType) {
        spirv_cross::Compiler compiler(data);
        spirv_cross::ShaderResources res = compiler.get_shader_resources();

        uint32_t index = 0;
        for (auto& [stype, s] : m_ShaderPathModules) {
            if (shaderType == stype)
                break;
            index++;
        }
        
        VkShaderStageFlagBits shaderStage = ShaderTypeToVkShaderStage(shaderType);

        SNOW_CORE_INFO("==========================");
        SNOW_CORE_INFO("VulkanShader::Reflect -  {0}", m_Paths[index]);
        SNOW_CORE_INFO("    {0} Inputs Attributes", res.stage_inputs.size());
        SNOW_CORE_INFO("    {0} Outputs Attributes", res.stage_outputs.size());
        SNOW_CORE_INFO("    {0} UBOs, {1} SSBOs, {2} Push Constants", res.uniform_buffers.size(), res.storage_buffers.size(), res.push_constant_buffers.size());
        SNOW_CORE_INFO("    {0} Images, {1} Storage Images", res.sampled_images.size(), res.storage_images.size());

        if (shaderType == Render::ShaderType::Vertex) {
            uint32_t stride = 0;
            uint32_t offset = 0;
            
            for (const spirv_cross::Resource& resource : res.stage_inputs) {
                //const auto& name = resource.name;
                auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
                auto location = compiler.get_decoration(resource.id, spv::DecorationLocation);
                auto type = compiler.get_type(resource.base_type_id);
                //auto ID = compiler.get_current_id_bound();
                //const auto& name = compiler.get_name(ID);
                uint32_t size = GetSizeFromSPIRType(type);
                
                auto& vertexInputAttrib = m_VertexInputAttributeDescriptions.emplace_back();
                vertexInputAttrib.binding = binding;
                vertexInputAttrib.location = location;
                vertexInputAttrib.format = SPIRTypeToVKFormat(type);
                vertexInputAttrib.offset = offset;

                offset += size;
            }
        }

        if (res.uniform_buffers.size())
            SNOW_CORE_TRACE("Uniform Buffers:");
        for (const spirv_cross::Resource& resource : res.uniform_buffers) {
            const auto& bufferName = resource.name;
            auto& bufferType = compiler.get_type(resource.base_type_id);
            int memberCount = bufferType.member_types.size();
            auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            auto descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto bufferSize = compiler.get_declared_struct_size(bufferType);

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            ShaderDescriptorSet& shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            //SNOW_CORE_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end());
            if (s_UniformBuffers[descriptorSet].find(binding) == s_UniformBuffers[descriptorSet].end()) {
                UniformBuffer* buffer = new UniformBuffer();
                buffer->BindingPoint = binding;
                buffer->Size = bufferSize;
                buffer->Name = bufferName;
                buffer->ShaderStage = VK_SHADER_STAGE_ALL;
                s_UniformBuffers.at(descriptorSet)[binding] = buffer;
            }
            else {
                UniformBuffer* buffer = s_UniformBuffers.at(descriptorSet).at(binding);
                if (bufferSize > buffer->Size)
                    buffer->Size = bufferSize;
            }

            shaderDescriptorSet.UniformBuffers[binding] = s_UniformBuffers.at(descriptorSet).at(binding);

            SNOW_CORE_TRACE("   {0} ({1}, {2})", bufferName, descriptorSet, binding);
            SNOW_CORE_TRACE("   Member Count: {0}", memberCount);
            SNOW_CORE_TRACE("   Size: {0}", bufferSize);
            SNOW_CORE_TRACE("-------------------------");
        }

        if (res.storage_buffers.size())
            SNOW_CORE_TRACE("Storage Buffers:");
        for (const spirv_cross::Resource& resource : res.storage_buffers) {
            const auto& bufferName = resource.name;
            auto& bufferType = compiler.get_type(resource.base_type_id);
            int memberCount = bufferType.member_types.size();
            auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            auto descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto bufferSize = compiler.get_declared_struct_size(bufferType);

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            ShaderDescriptorSet& shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            //SNOW_CORE_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end());
            if (s_StorageBuffers[descriptorSet].find(binding) == s_StorageBuffers[descriptorSet].end()) {
                StorageBuffer* buffer = new StorageBuffer();
                buffer->BindingPoint = binding;
                buffer->Size = bufferSize;
                buffer->Name = bufferName;
                buffer->ShaderStage = VK_SHADER_STAGE_ALL;
                s_StorageBuffers.at(descriptorSet)[binding] = buffer;
            }
            else {
                StorageBuffer* buffer = s_StorageBuffers.at(descriptorSet).at(binding);
                if (bufferSize > buffer->Size)
                    buffer->Size = bufferSize;
            }

            shaderDescriptorSet.StorageBuffers[binding] = s_StorageBuffers.at(descriptorSet).at(binding);

            SNOW_CORE_TRACE("   {0} ({1}, {2})", bufferName, descriptorSet, binding);
            SNOW_CORE_TRACE("   Member Count: {0}", memberCount);
            SNOW_CORE_TRACE("   Size: {0}", bufferSize);
            SNOW_CORE_TRACE("-------------------------");
        }

        if(res.push_constant_buffers.size())
            SNOW_CORE_TRACE("Push Constant Buffers:");
        for (const auto& resource : res.push_constant_buffers) {
            const auto& bufferName = resource.name;
            auto& bufferType = compiler.get_type(resource.base_type_id);
            auto bufferSize = compiler.get_declared_struct_size(bufferType);
            int memberCount = bufferType.member_types.size();
            uint32_t size = compiler.get_declared_struct_size(bufferType);
            uint32_t bufferOffset = 0;
            if (m_PushConstantRanges.size())
                bufferOffset = m_PushConstantRanges.back().Offset + m_PushConstantRanges.back().Size;

            auto& pushConstantRange = m_PushConstantRanges.emplace_back();
            pushConstantRange.ShaderStage = shaderStage;
            pushConstantRange.Size = bufferSize - bufferOffset;
            pushConstantRange.Offset = bufferOffset;

            if (bufferName.empty() || bufferName == "u_Renderer")
                continue;

            Render::ShaderBuffer& buffer = m_Buffers[bufferName];
            buffer.Name = bufferName;
            buffer.Size = bufferSize - bufferOffset;

            SNOW_CORE_TRACE("   Name: {0}", bufferName);
            SNOW_CORE_TRACE("   Member Count: {0}", memberCount);
            SNOW_CORE_TRACE("   Size: {0}", bufferSize);

            for (uint32_t i = 0; i < memberCount; i++) {
                auto memberType = compiler.get_type(bufferType.member_types[i]);
                const auto& memberName = compiler.get_member_name(bufferType.self, i);
                auto memberSize = compiler.get_declared_struct_member_size(bufferType, i);
                auto memberOffset = compiler.type_struct_member_offset(bufferType, i) - bufferOffset;

                std::string uniformName = fmt::format("{}.{}", bufferName, memberName);
                buffer.Uniforms[uniformName] = Render::ShaderUniform(uniformName, SPIRTypeToShaderUniformType(memberType), memberSize, memberOffset);
            }
        }

        if(res.sampled_images.size())
            SNOW_CORE_TRACE("Sampled images:");
        for (const auto& resource : res.sampled_images) {
            const auto& imageName = resource.name;
            auto& imageBaseType = compiler.get_type(resource.base_type_id);
            auto& imageType = compiler.get_type(resource.type_id);
            auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            auto descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32_t dimension = imageBaseType.image.dim;
            uint32_t arraySize = imageType.array[0];
            if (arraySize == 0)
                arraySize = 1;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            ShaderDescriptorSet& shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            auto& imageSampler = shaderDescriptorSet.ImageSamplers[binding];
            imageSampler.Name = imageName;
            imageSampler.BindingPoint = binding;
            imageSampler.DescriptorSet = descriptorSet;
            imageSampler.ShaderStage = shaderStage;
            imageSampler.ArraySize = arraySize;

            m_Resources[imageName] = Render::ShaderResource(imageName, shaderType, Render::ResourceType::Texture2D, binding, descriptorSet, arraySize);

            SNOW_CORE_TRACE("   {0} ({1}, {2})", imageName, descriptorSet, binding);
        }
        
        if(res.storage_images.size())
            SNOW_CORE_TRACE("Storage images:");
        for (const auto& resource : res.storage_images) {
            const auto& imageName = resource.name;
            auto& imageType = compiler.get_type(resource.base_type_id);
            auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            auto descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
            auto dimension = imageType.image.dim;

            if (descriptorSet >= m_ShaderDescriptorSets.size())
                m_ShaderDescriptorSets.resize(descriptorSet + 1);

            ShaderDescriptorSet& shaderDescriptorSet = m_ShaderDescriptorSets[descriptorSet];
            auto& imageSampler = shaderDescriptorSet.StorageImages[binding];
            imageSampler.Name = imageName;
            imageSampler.BindingPoint = binding;
            imageSampler.DescriptorSet = descriptorSet;
            imageSampler.ShaderStage = shaderStage;

            m_Resources[imageName] = Render::ShaderResource(imageName, shaderType, Render::ResourceType::Texture2D, binding, 1);

            SNOW_CORE_TRACE("   {0} ({1}, {2})", imageName, descriptorSet, binding);
        }
    }

    

    void VulkanShader::CreateDescriptors() {
        Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();

        m_TypeCounts.clear();
        for (uint32_t set = 0; set < m_ShaderDescriptorSets.size(); set++) {
            auto& shaderDescriptorSet = m_ShaderDescriptorSets[set];
            if (shaderDescriptorSet.UniformBuffers.size()) {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.UniformBuffers.size();
            }
            if (shaderDescriptorSet.StorageBuffers.size()) {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.UniformBuffers.size();
            }
            if (shaderDescriptorSet.ImageSamplers.size()) {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                typeCount.descriptorCount = shaderDescriptorSet.ImageSamplers.size();
            }
            if (shaderDescriptorSet.StorageImages.size()) {
                VkDescriptorPoolSize& typeCount = m_TypeCounts[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                typeCount.descriptorCount = shaderDescriptorSet.StorageImages.size();
            }

            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            for (auto& [binding, ubo] : shaderDescriptorSet.UniformBuffers) {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = ubo->ShaderStage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                VkWriteDescriptorSet& set = shaderDescriptorSet.WriteDescriptorSets[ubo->Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = 1;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto& [binding, storageBuffer] : shaderDescriptorSet.StorageBuffers) {
                VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = storageBuffer->ShaderStage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                VkWriteDescriptorSet& set = shaderDescriptorSet.WriteDescriptorSets[storageBuffer->Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = 1;
                set.dstBinding = layoutBinding.binding;
            }


            for (auto& [binding, imageSampler] : shaderDescriptorSet.ImageSamplers) {
                auto& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                layoutBinding.descriptorCount = imageSampler.ArraySize;
                layoutBinding.stageFlags = imageSampler.ShaderStage;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.binding = binding;

                SNOW_CORE_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
                SNOW_CORE_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");

                VkWriteDescriptorSet& set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = imageSampler.ArraySize;
                set.dstBinding = layoutBinding.binding;
            }

            for (auto& [bindingAndSet, imageSampler] : shaderDescriptorSet.StorageImages) {
                auto& layoutBinding = layoutBindings.emplace_back();
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags = imageSampler.ShaderStage;
                layoutBinding.pImmutableSamplers = nullptr;

                uint32_t binding = bindingAndSet & 0xffffffff;
                //uint32_t descriptorSet = (bindingAndSet >> 32);
                layoutBinding.binding = binding;

                SNOW_CORE_ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
                SNOW_CORE_ASSERT(shaderDescriptorSet.StorageBuffers.find(binding) == shaderDescriptorSet.StorageBuffers.end(), "Binding is already present!");
                SNOW_CORE_ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already present!");

                VkWriteDescriptorSet& set = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
                set = {};
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.descriptorType = layoutBinding.descriptorType;
                set.descriptorCount = 1;
                set.dstBinding = layoutBinding.binding;
            }

            VkDescriptorSetLayoutCreateInfo descriptorSetLayout = {};
            descriptorSetLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            descriptorSetLayout.pNext = nullptr;
            descriptorSetLayout.bindingCount = layoutBindings.size();
            descriptorSetLayout.pBindings = layoutBindings.data();

            SNOW_CORE_INFO("Creating Descriptor set {0} with {1} ubos, {2} ssbos, {3} samplers and {4} storage images", set,
                shaderDescriptorSet.UniformBuffers.size(),
                shaderDescriptorSet.StorageBuffers.size(),
                shaderDescriptorSet.ImageSamplers.size(),
                shaderDescriptorSet.StorageImages.size());

            if (set >= m_DescriptorSetLayouts.size()) {
                m_DescriptorSetLayouts.resize((size_t)(set + 1));
            }

            VKCheckError(vkCreateDescriptorSetLayout(vkDevice->GetVulkanDevice(), &descriptorSetLayout, nullptr, &m_DescriptorSetLayouts[set]));
        }
        // Move to global renderer
        /*
        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.poolSizeCount = typeCounts.size();
        descriptorPoolInfo.pPoolSizes = typeCounts.data();
        descriptorPoolInfo.maxSets = 1;

        VKCheckError(vkCreateDescriptorPool(vkDevice->GetVulkanDevice(), &descriptorPoolInfo, nullptr, &m_DescriptorPool));
        */
    }

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set) {
        ShaderMaterialDescriptorSet result;

        Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();
        SNOW_CORE_ASSERT(m_TypeCounts.find(set) != m_TypeCounts.end());

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.poolSizeCount = m_TypeCounts.at(set).size();
        descriptorPoolInfo.pPoolSizes = m_TypeCounts.at(set).data();
        descriptorPoolInfo.maxSets = 1;

        VKCheckError(vkCreateDescriptorPool(vkDevice->GetVulkanDevice(), &descriptorPoolInfo, nullptr, &result.Pool));

        // Allocating a new descriptor set from the descriptor pool
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = result.Pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

        result.DescriptorSets.emplace_back();
        VKCheckError(vkAllocateDescriptorSets(vkDevice->GetVulkanDevice(), &allocInfo, result.DescriptorSets.data()));
        return result;
    }

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set, uint32_t numOfSets) {
        ShaderMaterialDescriptorSet result;

        Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();

        std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> poolSizes;
        for (uint32_t set = 0; set < m_ShaderDescriptorSets.size(); set++) {
            auto& shaderDescriptorSet = m_ShaderDescriptorSets[set];
            if (shaderDescriptorSet.UniformBuffers.size()) {
                VkDescriptorPoolSize& typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.UniformBuffers.size() * numOfSets;
            }
            if (shaderDescriptorSet.StorageBuffers.size()) {
                VkDescriptorPoolSize& typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                typeCount.descriptorCount = shaderDescriptorSet.StorageBuffers.size() * numOfSets;
            }
            if (shaderDescriptorSet.ImageSamplers.size()) {
                VkDescriptorPoolSize& typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                uint32_t descriptorSetCount = 0;
                for (auto&& [binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
                    descriptorSetCount += imageSampler.ArraySize;

                typeCount.descriptorCount = descriptorSetCount * numOfSets;
            }
            if (shaderDescriptorSet.StorageImages.size()) {
                VkDescriptorPoolSize& typeCount = poolSizes[set].emplace_back();
                typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                typeCount.descriptorCount = shaderDescriptorSet.StorageImages.size() * numOfSets;
            }
        }

        SNOW_CORE_ASSERT(poolSizes.find(set) != poolSizes.end());

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.poolSizeCount = poolSizes.at(set).size();
        descriptorPoolInfo.pPoolSizes = poolSizes.at(set).data();
        descriptorPoolInfo.maxSets = numOfSets;

        VKCheckError(vkCreateDescriptorPool(vkDevice->GetVulkanDevice(), &descriptorPoolInfo, nullptr, &result.Pool));

        for (uint32_t i = 0; i < numOfSets; i++) {
            VkDescriptorSetAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = result.Pool;
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];

            VKCheckError(vkAllocateDescriptorSets(vkDevice->GetVulkanDevice(), &allocInfo, &result.DescriptorSets[i]));
        }
        return result;
    }

    VulkanShader::ShaderMaterialDescriptorSet VulkanShader::AllocateDescriptorSet(uint32_t set) {
        SNOW_CORE_ASSERT(set < m_DescriptorSetLayouts.size());
        ShaderMaterialDescriptorSet result;

        if (m_ShaderDescriptorSets.empty())
            return result;

        result.Pool = nullptr;

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_DescriptorSetLayouts[set];
        VkDescriptorSet descriptorSet = Render::VulkanRenderer::RTAllocateDescriptorSet(allocInfo);
        SNOW_CORE_ASSERT(descriptorSet);
        result.DescriptorSets.push_back(descriptorSet);
        return result;
    }

    const VkWriteDescriptorSet* VulkanShader::GetDescriptorSet(const std::string& name, uint32_t set) const {
        SNOW_CORE_ASSERT(set<m_ShaderDescriptorSets.size());
        SNOW_CORE_ASSERT(m_ShaderDescriptorSets[set]);
        if (m_ShaderDescriptorSets.at(set).WriteDescriptorSets.find(name) == m_ShaderDescriptorSets.at(set).WriteDescriptorSets.end()) {
            SNOW_CORE_WARN("Shader {0} does not contain requested descriptor set {1}", m_Name, name);
            return nullptr;
        }
        return &m_ShaderDescriptorSets.at(set).WriteDescriptorSets.at(name);
    }

    std::vector<VkDescriptorSetLayout> VulkanShader::GetAllDescriptorSetLayouts() {
        std::vector<VkDescriptorSetLayout> result;
        result.reserve(m_DescriptorSetLayouts.size());
        for (auto& layout : m_DescriptorSetLayouts)
            result.emplace_back(layout);
        return result;
    }
}