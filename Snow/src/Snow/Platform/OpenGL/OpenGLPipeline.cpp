#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLPipeline.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

#include <spirv_glsl.hpp>

namespace Snow {
    namespace Render {

        OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec) :
            m_Specification(spec) {

            glGenVertexArrays(1, &m_PipelineVertexArrayHandle);
            glBindVertexArray(m_PipelineVertexArrayHandle);

            LinkShaders();

            Reflect();
        }

        void OpenGLPipeline::Bind() {
            glUseProgram(m_PipelineHandle);
            
            uint32_t layoutIndex = 0;
            for(const auto& element : m_Specification.Layout) {
                glEnableVertexAttribArray(layoutIndex);
                glVertexAttribPointer(layoutIndex, element.GetComponentCount(), OpenGLBufferAttribType(element.Type), element.Normalized? GL_TRUE: GL_FALSE, m_Specification.Layout.GetStride(), (const void*)(intptr_t)element.Offset);
                layoutIndex++;
            }
        }

        GLenum OpenGLPipeline::OpenGLBufferAttribType(AttribType type) const {
            switch(type) {
            case AttribType::Float: return GL_FLOAT;
            case AttribType::Float2: return GL_FLOAT;
            case AttribType::Float3: return GL_FLOAT;
            case AttribType::Float4: return GL_FLOAT;
            case AttribType::Int: return GL_INT;
            case AttribType::Int2: return GL_INT;
            case AttribType::Int3: return GL_INT;
            case AttribType::Int4: return GL_INT;
            }
            SNOW_CORE_ERROR("Unknown AttribType for glVertexAttribPointer");
            return 0;
        }

        void OpenGLPipeline::LinkShaders() {

            m_PipelineHandle = glCreateProgram();
            SNOW_CORE_TRACE("Created program for pipeline");
            int glShaderIDIndex = 0;
            for(uint32_t i=0; i< m_Specification.Shaders.size(); i++) {
                Ref<OpenGLShader> glShader = m_Specification.Shaders[i].As<OpenGLShader>();
                glAttachShader(m_PipelineHandle, glShader->GetShaderID());
            }

            glLinkProgram(m_PipelineHandle);
            GLint isLinked = 0;
            glGetProgramiv(m_PipelineHandle, GL_LINK_STATUS, &isLinked);
            if(isLinked == GL_FALSE) {
                GLint maxLength = 0;
			    glGetProgramiv(m_PipelineHandle, GL_INFO_LOG_LENGTH, &maxLength);

			    std::vector<GLchar> infoLog(maxLength);
			    glGetProgramInfoLog(m_PipelineHandle, maxLength, &maxLength, &infoLog[0]);
			    SNOW_CORE_ERROR("Program linking failed:\n{0}", &infoLog[0]);

			    glDeleteProgram(m_PipelineHandle);

			    for (uint32_t i = 0; i < m_Specification.Shaders.size(); i++) {
			    	Ref<OpenGLShader> glShader = m_Specification.Shaders[i].As<OpenGLShader>();
			    	glDeleteShader(glShader->GetShaderID());
			    	glDetachShader(m_PipelineHandle, glShader->GetShaderID());
			    }
            }
        }

        void OpenGLPipeline::Reflect() {
            glUseProgram(m_PipelineHandle);

            for(const auto& shader : m_Specification.Shaders){
                Ref<OpenGLShader> glShader = static_cast<Ref<OpenGLShader>>(shader);
                spirv_cross::CompilerGLSL compiler(glShader->GetSPIRVBinaryData());
                spirv_cross::ShaderResources res = compiler.get_shader_resources();

                SNOW_CORE_INFO("==========================");
                SNOW_CORE_INFO("Shader file {0}", glShader->GetPath());
                //SNOW_CORE_INFO("    {0} Inputs in current stage", res.stage_inputs.size());
                //SNOW_CORE_INFO("    {0} Uniform buffers", res.uniform_buffers.size());
               // SNOW_CORE_INFO("    {0} Resources", res.sampled_images.size());

                uint32_t inputAttribIndex = 0;
                for(const auto inputAttrib : res.stage_inputs) {
                    auto attribType = compiler.get_type(inputAttrib.type_id);
                    

                    //SNOW_CORE_TRACE("   Input attrib type {0}", attribType);
                }

                uint32_t bufferIndex = 0;
                for(const spirv_cross::Resource& resource : res.uniform_buffers) {
                    auto& bufferType = compiler.get_type(resource.base_type_id);
                    int memberCount = bufferType.member_types.size();
                    uint32_t bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
                    auto& name = resource.name;
                    //SNOW_CORE_TRACE("Uniform {0} bound at bindingPoint {1}", name, bindingPoint);
                    //if(s_UniformBuffers.find(bindingPoint) == s_UniformBuffers.end()) {
//
                    //}
                }

                int32_t sampler = 0;
                for(const spirv_cross::Resource& resource : res.sampled_images) {
                    
                    auto& type = compiler.get_type(resource.type_id);
                    auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
                    const auto& name = resource.name;
                    //SNOW_CORE_INFO("    Resource Name {0}", name.c_str());
                    uint32_t dimension = type.image.dim;

                    GLint location = glGetUniformLocation(m_PipelineHandle, name.c_str());
                    m_Resources[name] = ShaderResource(name, binding, 1);
                    glUniform1i(location, binding);

                    //SNOW_CORE_INFO("   Binding resource {0}, at location {1}", name, location);

                    
                }

                //SNOW_CORE_INFO("=======================");
                //SNOW_CORE_INFO("");
            }
        }

    }
}