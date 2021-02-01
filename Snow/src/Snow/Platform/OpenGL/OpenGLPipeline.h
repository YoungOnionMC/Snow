#pragma once
#include "Snow/Render/Pipeline.h"



#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Snow {
    namespace Render {
        class OpenGLPipeline : public Pipeline {
        public:
            OpenGLPipeline(const PipelineSpecification& spec);

            void Bind() const override;



            const PipelineSpecification& GetSpecification() const override { return m_Specification; }

        private:
            
            GLenum OpenGLBufferAttribType(AttribType type) const;
            //void ReflectStruct(ShaderStruct& sstruct, spirv_cross::Resource resource);

            



            PipelineSpecification m_Specification;

            uint32_t m_PipelineVertexArrayHandle;
            uint32_t m_PipelineHandle;

            
        };
    }
}