#pragma once
#include "Snow/Render/Pipeline.h"

#include "Snow/Render/Shader/ShaderUniform.h"

#include <glad/glad.h>

#include "Snow/Math/Math.h"

namespace Snow {
    namespace Render {
        class OpenGLPipeline : public Pipeline {
        public:
            OpenGLPipeline(const PipelineSpecification& spec);

            void Bind() override;

            const PipelineSpecification& GetSpecification() const override { return m_Specification; }

        private:
            void LinkShaders();

            void Reflect();

            GLint GetUniformLocation(const std::string& name);

            GLenum OpenGLBufferAttribType(AttribType type) const;

            void SetUniform(const std::string& name, int value);
            void SetUniform(const std::string& name, float value);
            void SetUniform(const std::string& name, const Math::Vector2f& value);
            void SetUniform(const std::string& name, const Math::Vector3f& value);
            void SetUniform(const std::string& name, const Math::Vector4f& value);

            void SetUniformIntArray(const std::string& name, int* values, uint32_t size);

            void UploadUniformInt(uint32_t location, int32_t value);
            void UploadUniformIntArray(uint32_t location, int32_t* values, uint32_t count);
            void UploadUniformFloat(uint32_t location, float value);
            void UploadUniformFloat2(uint32_t location, const Math::Vector2f& value);
            void UploadUniformFloat3(uint32_t location, const Math::Vector3f& value);
            void UploadUniformFloat4(uint32_t location, const Math::Vector4f& value);



            PipelineSpecification m_Specification;

            uint32_t m_PipelineVertexArrayHandle;
            uint32_t m_PipelineHandle;


            std::unordered_map<uint32_t, ShaderUniformBuffer> m_UniformBuffers;
            std::unordered_map<std::string, ShaderBuffer> m_Buffers;
            std::unordered_map<std::string, ShaderResource> m_Resources;
            std::unordered_map<std::string, GLint> m_UniformLocations;
        };
    }
}