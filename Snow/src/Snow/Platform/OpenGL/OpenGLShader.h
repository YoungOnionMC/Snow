#pragma once

#include "Snow/Render/Shader/Shader.h"
#include "Snow/Render/Shader/ShaderUniform.h"

#include <glad/glad.h>

#include <shaderc/shaderc.hpp>

#include <glm/glm.hpp>

namespace Snow {
    namespace Render {
        class OpenGLShader : public Shader {
        public:
            OpenGLShader(const std::initializer_list<ShaderModule>& shaderModules);

            void Bind() const override;
            void Reload() override;

            virtual const ShaderUniformBuffer& GetUniformBuffer(const std::string& name) const override;
            virtual const std::unordered_map<std::string, ShaderResource>& GetResources() const override { return m_Resources; }

            virtual void SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size) override;
            virtual void SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) override;

            uint32_t GetShaderID() const { return m_RendererID; }

            //const ShaderType GetType() const override { return m_Type; }

            const std::string& GetPath() const override { return m_Paths[0]; }
            const std::string& GetName() const override { return m_Name; }

            //const std::vector<uint32_t>& GetSPIRVBinaryData() const { return m_SPIRVBinaryData; }
            //const std::vector<uint32_t>& GetGLSLBinaryData() const { return m_GLSLBinaryData; }
            //const std::vector<std::string>& GetGLSLSourceDataString() const { return m_GLSLSourceData; }

        private:

            shaderc_shader_kind SnowShaderTypeToShaderC(ShaderType type);

            void CreateSPIRVBinaryCache(uint32_t shaderIndex);
            void CreateGLSLBinaryCache(uint32_t shaderIndex);
            void CreateOpenGLShaderModule(uint32_t shaderIndex, bool spirvModule);

            void LinkShaders();

            void SPIRVReflection();
            void OpenGLReflection();
            void GLSLReflect();

            GLenum GetShaderType(ShaderType type);
            ShaderType ShaderTypeFromString(const std::string& type);
            std::string ShaderTypeToString(ShaderType type);

            void Load();
            void PreProcess(const std::string& source);

            std::string ReadShaderFromFile(const std::string& path);

            uint32_t GetUniformLocation(const std::string& name);

            void SetUniform(const std::string& name, int value);
            void SetUniform(const std::string& name, uint32_t value);
            void SetUniform(const std::string& name, float value);
            void SetUniform(const std::string& name, const glm::vec2& value);
            void SetUniform(const std::string& name, const glm::vec3& value);
            void SetUniform(const std::string& name, const glm::vec4& value);

            void SetUniformIntArray(const std::string& name, int* values, uint32_t size);

            void UploadUniformInt(uint32_t location, int32_t value);
            void UploadUniformIntArray(uint32_t location, int32_t* values, uint32_t count);
            void UploadUniformFloat(uint32_t location, float value);
            void UploadUniformFloat2(uint32_t location, const glm::vec2& value);
            void UploadUniformFloat3(uint32_t location, const glm::vec3& value);
            void UploadUniformFloat4(uint32_t location, const glm::vec4& value);

            

            std::vector<ShaderModule> m_ShaderModules;
            std::string m_Name;
            std::vector<std::string> m_Paths;
            std::vector<std::string> m_ShaderSources;
            std::vector<ShaderType> m_ShaderTypes;

            uint32_t m_RendererID = 0;
            std::vector<uint32_t> m_ShaderIDs;

            bool m_Reloaded = false;

            std::vector<std::vector<uint32_t>> m_SPIRVBinaryData; // vector of uint32_t is the spirv module, vector of modules
            std::vector<std::vector<uint32_t>> m_GLSLBinaryData; // vector of uint32_t is the glsl module, vector of those
            std::vector<std::vector<std::string>> m_GLSLSourceData; // vector of string is the file, line by line,  vector of those

            inline static std::unordered_map<uint32_t, ShaderUniformBuffer> m_UniformBuffers;
            std::unordered_map<std::string, ShaderBuffer> m_Buffers;
            std::unordered_map<std::string, ShaderResource> m_Resources;
            std::unordered_map<std::string, GLint> m_UniformLocations;
        };
    }
}