#pragma once
#include "Snow/Render/Pipeline.h"

#include "Snow/Render/Shader/ShaderUniform.h"

#include <glad/glad.h>

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

            GLenum OpenGLBufferAttribType(AttribType type) const;

            PipelineSpecification m_Specification;

            uint32_t m_PipelineVertexArrayHandle;
            uint32_t m_PipelineHandle;



            //std::unordered_map<std::string, ShaderBuffer> m_Buffers;
            std::unordered_map<std::string, ShaderResource> m_Resources;
            std::unordered_map<std::string, GLint> m_UniformLocations;
        };
    }
}