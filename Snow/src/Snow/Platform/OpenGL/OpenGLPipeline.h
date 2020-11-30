#pragma once
#include "Snow/Render/Pipeline.h"

#include "Snow/Render/Shader/ShaderUniform.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Snow {
    namespace Render {
        class OpenGLPipeline : public Pipeline {
        public:
            OpenGLPipeline(const PipelineSpecification& spec);

            void Bind() override;

            void SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size) override;
            void SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) override;

            const PipelineSpecification& GetSpecification() const override { return m_Specification; }

        private:
            void LinkShaders();

            void SPIRVReflection();
            void OpenGLReflection();

            uint32_t GetUniformLocation(const std::string& name);

            GLenum OpenGLBufferAttribType(AttribType type) const;

            void SetUniform(const std::string& name, int value);
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



            PipelineSpecification m_Specification;

            uint32_t m_PipelineVertexArrayHandle;
            uint32_t m_PipelineHandle;

            inline static std::unordered_map<uint32_t, ShaderUniformBuffer> m_UniformBuffers;
            std::unordered_map<std::string, ShaderBuffer> m_Buffers;
            std::unordered_map<std::string, ShaderResource> m_Resources;
            std::unordered_map<std::string, GLint> m_UniformLocations;
        };
    }
}