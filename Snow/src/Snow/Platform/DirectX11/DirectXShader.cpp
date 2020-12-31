#include <spch.h>

#include "Snow/Platform/DirectX11/DirectXShader.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include <shaderc.hpp>
#include <spirv_hlsl.hpp>

namespace Snow {

    static const std::unordered_map<Render::ShaderType, uint32_t> SHADERBLOB = {
        {Render::ShaderType::Vertex, 0},
        {Render::ShaderType::Pixel, 4},
        {Render::ShaderType::Compute, 5},
        {Render::ShaderType::Geometry, 1}
    };

	DirectX11Shader::DirectX11Shader(Render::ShaderType type, const std::string& path) :
        m_Path(path), m_Type(type) {
		size_t found = path.find_last_of("/\\");
		m_Name = found != std::string::npos ? path.substr(found + 1) : path;
		found = m_Name.find_last_of(".");
		m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

		m_Source = ReadShaderFromFile(m_Path);
        //CreateSPIRVBinaryCache();
        //CreateHLSLBinaryCache();

        CreateDirectXShaderModule(m_Blob.of[SHADERBLOB.at(m_Type)], false);

	}

    std::string DirectX11Shader::ReadShaderFromFile(const std::string& path) {
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

    void DirectX11Shader::CreateSPIRVBinaryCache() {


        /*
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
            options.SetOptimizationLevel(shaderc_optimization_level_zero);

            const bool optimize = false;
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

            //SNOW_CORE_TRACE("Size of shader {0}", m_SPIRVBinaryData.size());


        }
        */
    }

    static const std::unordered_map<Render::ShaderType, const char*> SHADERVERSION = {
        {Render::ShaderType::Vertex, "vs_5_0"},
        {Render::ShaderType::Pixel, "ps_5_0"},
        {Render::ShaderType::Compute, "cs_5_0"},
        {Render::ShaderType::Geometry, "gs_5_0"}
    };

    std::string SnowToHLSLTarget(Render::ShaderType type) {
        return SHADERVERSION.at(type);
    }

    std::wstring stringToWString(const std::string& s) {
        return std::wstring(s.begin(), s.end());
    }

    void DirectX11Shader::CreateDirectXShaderModule(ID3D10Blob*& blob, bool spirvmodule) {
        ID3D10Blob* errorBlob = nullptr;

        auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

        auto wstr = std::wstring{ m_Path.begin(), m_Path.end() };
        const WCHAR* pathStr = wstr.c_str();


        D3DCompileFromFile(pathStr, 
            NULL, NULL, 
            "main", 
            SnowToHLSLTarget(m_Type).c_str(), 
            D3DCOMPILE_DEBUG, 0, 
            &blob,
            &errorBlob);

        std::string message;
        std::string profile = SnowToHLSLTarget(m_Type);
        if (errorBlob) {
            if (errorBlob->GetBufferSize())
                SNOW_CORE_ERROR("Shader compilation error (shader {0}) : {1}", profile, (const char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        
        switch (m_Type) {
        case Render::ShaderType::Vertex: {
            dxDevice->GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_Stages.vs);
            break;
        }
        case Render::ShaderType::Pixel: {
            dxDevice->GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_Stages.ps);
            break;
        }
        case Render::ShaderType::Geometry: {
            dxDevice->GetDevice()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_Stages.gs);
            break;
        }
        case Render::ShaderType::Compute: {
            dxDevice->GetDevice()->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_Stages.cs);
            break;
        }
        }

        
    }

    ID3D10Blob* DirectX11Shader::GetBlob(Render::ShaderType type) {
        return m_Blob.of[SHADERBLOB.at(type)];
    }
}