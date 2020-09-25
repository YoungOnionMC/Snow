#pragma once
#include "Snow/Render/Pipeline.h"

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

            GLenum OpenGLBufferAttribType(AttribType type) const;

            PipelineSpecification m_Specification;

            uint32_t m_PipelineVertexArrayHandle;
            uint32_t m_PipelineHandle;
        };
    }
}