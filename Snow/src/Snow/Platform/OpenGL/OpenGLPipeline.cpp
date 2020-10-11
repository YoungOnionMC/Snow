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

            SPIRVReflection();
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

        static UniformType SPIRVTypeToShaderUniformType(spirv_cross::SPIRType type) {
            switch (type.basetype) {
            case spirv_cross::SPIRType::Boolean:    return UniformType::Bool;
            case spirv_cross::SPIRType::Int: {
                if (type.vecsize == 1)   return UniformType::Int;
                if (type.vecsize == 2)   return UniformType::Int2;
                if (type.vecsize == 3)   return UniformType::Int3;
                if (type.vecsize == 4)   return UniformType::Int4;
            }
            case spirv_cross::SPIRType::Float: {
                if (type.vecsize == 1)   return UniformType::Float;
                if (type.vecsize == 2)   return UniformType::Float2;
                if (type.vecsize == 3)   return UniformType::Float3;
                if (type.vecsize == 4)   return UniformType::Float4;

                if (type.columns == 3 && type.vecsize == 3)  return UniformType::Mat3x3;
                if (type.columns == 4 && type.vecsize == 4)  return UniformType::Mat4x4;
            }
            }
            return UniformType::None;
        }

        void OpenGLPipeline::LinkShaders() {

            m_PipelineHandle = glCreateProgram();
            SNOW_CORE_TRACE("Created program for pipeline");
            for(auto& shader : m_Specification.Shaders) {
                Ref<OpenGLShader> glShader = shader.As<OpenGLShader>();
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

			    for (auto& shader : m_Specification.Shaders) {
			    	Ref<OpenGLShader> glShader = shader.As<OpenGLShader>();
			    	glDeleteShader(glShader->GetShaderID());
			    	glDetachShader(m_PipelineHandle, glShader->GetShaderID());
			    }
            }
            for (auto& shader : m_Specification.Shaders) {
                Ref<OpenGLShader> glShader = shader.As<OpenGLShader>();
                glDetachShader(m_PipelineHandle, glShader->GetShaderID());
            }
        }

        void OpenGLPipeline::SPIRVReflection() {
            glUseProgram(m_PipelineHandle);

            std::vector<spirv_cross::SPIRType> outputAttributes;

            for (auto& shader : m_Specification.Shaders) {
                Ref<OpenGLShader> glShader = shader.As<OpenGLShader>();
                //spirv_cross::Compiler compiler(glShader->GetSPIRVBinaryData());
                spirv_cross::Compiler compiler(glShader->GetGLSLBinaryData());
                spirv_cross::ShaderResources res = compiler.get_shader_resources();

                SNOW_CORE_INFO("==========================");
                SNOW_CORE_INFO("Shader file {0}", glShader->GetPath());
                SNOW_CORE_INFO("    {0} Inputs Attributes", res.stage_inputs.size());
                SNOW_CORE_INFO("    {0} Outputs Attributes", res.stage_outputs.size());
                SNOW_CORE_INFO("    {0} Uniform buffers", res.uniform_buffers.size());
                SNOW_CORE_INFO("    {0} Resources", res.sampled_images.size());

                uint32_t inputAttribIndex = 0;
                if (shader->GetType() == ShaderType::Vertex) {
                    for (const auto& inputAttrib : res.stage_inputs) {
                        const auto& element = m_Specification.Layout.GetElements()[inputAttribIndex];
                        auto attribType = compiler.get_type(inputAttrib.type_id);

                        // BaseType 13 is for floats, ie input attributes for any shader
                        if (attribType.basetype == 13 && element.GetComponentCount() != attribType.vecsize) {
                            SNOW_CORE_WARN("Attrib {0} from the pipeline does not match the bound shaders", inputAttribIndex);
                        }

                        auto location = compiler.get_decoration(inputAttrib.id, spv::DecorationLocation);
                        SNOW_CORE_TRACE("Location of input attrib {0}", location);

                        inputAttribIndex++;
                    }
                }

                uint32_t outputAttribIndex = 0;
                for (const auto& outputAttrib : res.stage_outputs) {
                    auto attribType = compiler.get_type(outputAttrib.type_id);
                    auto binding = compiler.get_decoration(outputAttrib.id, spv::DecorationBinding);

                    SNOW_CORE_TRACE("Found An Output {0}", binding);
                }

                for (const spirv_cross::Resource& resource : res.push_constant_buffers) {
                    auto& bufferType = compiler.get_type(resource.base_type_id);
                    auto location = compiler.get_decoration(resource.id, spv::DecorationLocation);
                    const auto& bufferName = resource.name;
                    auto bufferSize = compiler.get_declared_struct_size(bufferType);
                    int memberCount = bufferType.member_types.size();

                    ShaderBuffer& buffer = m_Buffers[bufferName];
                    buffer.Name = bufferName;
                    buffer.Size = bufferSize;
                    for (uint32_t i = 0; i < memberCount; i++) {
                        auto type = compiler.get_type(bufferType.member_types[i]);
                        const auto& memberName = compiler.get_member_name(bufferType.self, i);
                        auto size = compiler.get_declared_struct_member_size(bufferType, i);
                        auto offset = compiler.type_struct_member_offset(bufferType, i);

                        std::string uniformName = bufferName + "." + memberName;
                        buffer.Uniforms[uniformName] = ShaderUniform(uniformName, SPIRVTypeToShaderUniformType(type), size, offset);
                    }
                }

                uint32_t bufferIndex = 0;
                for(const spirv_cross::Resource& resource : res.uniform_buffers) {
                    auto& bufferType = compiler.get_type(resource.base_type_id);
                    int memberCount = bufferType.member_types.size();
                    uint32_t bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);

                    if (m_UniformBuffers.find(bindingPoint) == m_UniformBuffers.end()) {
                        ShaderUniformBuffer& buffer = m_UniformBuffers[bindingPoint];
                        buffer.Name = resource.name;
                        buffer.BindingPoint = bindingPoint;
                        buffer.Size = compiler.get_declared_struct_size(bufferType);

                        // move uniform buffers to thier own class
                        glCreateBuffers(1, &buffer.RendererID);
                        glBindBuffer(GL_UNIFORM_BUFFER, buffer.RendererID);
                        glBufferData(GL_UNIFORM_BUFFER, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
                        glBindBufferBase(GL_UNIFORM_BUFFER, buffer.BindingPoint, buffer.RendererID);

                        SNOW_CORE_TRACE("Created Uniform buffer {0} at binding point {1}, size is {2} bytes", buffer.Name, buffer.BindingPoint, buffer.Size);

                        glBindBuffer(GL_UNIFORM_BUFFER, 0);
                    }
                }

                int32_t sampler = 0;
                for(const spirv_cross::Resource& resource : res.sampled_images) {
                    const auto& resourceType = compiler.get_type(resource.type_id);
                    auto binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
                    const auto& name = resource.name;

                    int location = compiler.get_decoration(resource.id, spv::DecorationLocation);
                    
                    int arrayCount = 0;
                    int* samplers;
                    if (resourceType.array.size() == 1) {
                        arrayCount = resourceType.array[0];
                        samplers = new int[arrayCount];

                        for (uint32_t i = 0; i < arrayCount; i++) {
                            samplers[i] = binding + i;
                        }
                    }
                    else if (resourceType.array.size() == 0)
                        arrayCount = 1;


                    

                    
                    m_Resources[name] = ShaderResource(name, binding, arrayCount);
                    if (arrayCount > 1);
                        //UploadUniformIntArray(location, samplers, arrayCount);
                    else if (arrayCount == 1);
                        //UploadUniformInt(location, binding);

                    //SNOW_CORE_INFO("   Binding resource {0}, at location {1}", name, location);

                    
                }

                //SNOW_CORE_INFO("=======================");
                //SNOW_CORE_INFO("");
            }
        }

        void OpenGLPipeline::OpenGLReflection() {
            glUseProgram(m_PipelineHandle);


        }

        uint32_t OpenGLPipeline::GetUniformLocation(const std::string& name) {
            GLint GLlocation = glGetUniformLocation(m_PipelineHandle, name.c_str());

            if (GLlocation == -1) {
                SNOW_CORE_WARN("Uniform {0} could not be found in current pipeline", name);
            }
            return (uint32_t)GLlocation;
        }

        void OpenGLPipeline::SetUniform(const std::string& name, int value) {
            glUniform1i(GetUniformLocation(name), value);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, float value) {
            glUniform1f(GetUniformLocation(name), value);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const Math::Vector2f& value) {
            glUniform2f(GetUniformLocation(name), value.x, value.y);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const Math::Vector3f& value) {
            glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const Math::Vector4f& value) {
            glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
        }

        void OpenGLPipeline::SetUniformIntArray(const std::string& name, int* values, uint32_t count) {
            glUniform1iv(GetUniformLocation(name), count, values);
        }

        void OpenGLPipeline::UploadUniformInt(uint32_t location, int32_t value) {
            glUniform1i(location, value);
        }

        void OpenGLPipeline::UploadUniformFloat(uint32_t location, float value) {
            glUniform1f(location, value);
        }

        void OpenGLPipeline::UploadUniformFloat2(uint32_t location, const Math::Vector2f& value) {
            glUniform2f(location, value.x, value.y);
        }

        void OpenGLPipeline::UploadUniformFloat3(uint32_t location, const Math::Vector3f& value) {
            glUniform3f(location, value.x, value.y, value.z);
        }

        void OpenGLPipeline::UploadUniformFloat4(uint32_t location, const Math::Vector4f& value) {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }

        void OpenGLPipeline::UploadUniformIntArray(uint32_t location, int32_t* values, uint32_t count) {
            glUniform1iv(location, count, values);
        }
    }
}