#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLPipeline.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

#include <spirv_glsl.hpp>

#include <shaderc/shaderc.hpp>

namespace Snow {
    namespace Render {

        OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec) :
            m_Specification(spec) {

            glGenVertexArrays(1, &m_PipelineVertexArrayHandle);
            glBindVertexArray(m_PipelineVertexArrayHandle);

            LinkShaders();

            SPIRVReflection();
        }

        void OpenGLPipeline::Bind() const {
            glUseProgram(m_PipelineHandle);
            
            uint32_t layoutIndex = 0;
            for(const auto& element : m_Specification.Layout) {
                glEnableVertexAttribArray(layoutIndex);
                glVertexAttribPointer(layoutIndex, element.GetComponentCount(), OpenGLBufferAttribType(element.Type), element.Normalized? GL_TRUE: GL_FALSE, m_Specification.Layout.GetStride(), (const void*)(intptr_t)element.Offset);
                layoutIndex++;
            }

            //SNOW_CORE_ERROR("UNIFORM BUFFER SIZE : {0}", m_UniformBuffers.size());
        }

        const ShaderUniformBuffer& OpenGLPipeline::GetUniformBuffer(const std::string& name) const {
            for (auto& [bindingPoint, ub] : m_UniformBuffers) {
                if (ub.Name == name) {
                    return ub;
                }
            }
            SNOW_CORE_WARN("Could not find uniform buffer, returning UBO 0");
            return m_UniformBuffers[0];
        }
        /*
        const ShaderResource OpenGLPipeline::GetResource(const std::string& name) const {
            for (auto& [name, resource] : m_Resources) {
                if (resource.GetName() == name)
                    return resource;
            }
            SNOW_CORE_ERROR("Could not find resource");
            return {};
        }*/

        void OpenGLPipeline::SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size) {
            uint8_t* buffer = new uint8_t[size];
            memcpy(buffer, data, size);
            
            ShaderUniformBuffer* uniformBuffer = nullptr;
            for(auto& [bindingPoint, ub] : m_UniformBuffers) {
                if (ub.Name == uniformBufferName) {
                    uniformBuffer = &ub;
                    break;
                }
            }
            if (uniformBuffer == nullptr)
                return;

            glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->RendererID);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, buffer);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            delete[] buffer;
        }

        void OpenGLPipeline::SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) {
            uint8_t* buffer = new uint8_t[size];
            memcpy(buffer, data, size);


            ShaderUniformBuffer* uniformBuffer = &m_UniformBuffers[bindingPoint];

            glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->RendererID);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, buffer);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            delete[] buffer;
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
                spirv_cross::Compiler compiler(glShader->GetSPIRVBinaryData());
                //spirv_cross::CompilerGLSL compilerGLSL(glShader->GetGLSLBinaryData());
                spirv_cross::ShaderResources res = compiler.get_shader_resources();
                //spirv_cross::ShaderResources res2 = compilerGLSL.get_shader_resources();

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
                    auto location = compiler.get_decoration(outputAttrib.id, spv::DecorationLocation);

                    SNOW_CORE_TRACE("Found An Output binding: {0}, location: {1}", binding, location);
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

                        uint32_t uniformCount = compiler.get_type(resource.base_type_id).member_types.size();
                        for (uint32_t i = 0; i < uniformCount; i++) {
                            std::string name = compiler.get_member_name(resource.base_type_id, i);
                            UniformType type = SPIRVTypeToShaderUniformType(compiler.get_type(compiler.get_type(resource.base_type_id).member_types[i]));
                            uint32_t size = compiler.get_declared_struct_member_size(compiler.get_type(resource.base_type_id), i);
                            uint32_t offset = compiler.type_struct_member_offset(compiler.get_type(resource.base_type_id), i);
                            
                            ShaderStruct sstruct = ShaderStruct(name);
                            if (type == UniformType::Struct) {
                                auto structType = compiler.get_type(compiler.get_type(resource.base_type_id).member_types[i]);


                                spirv_cross::TypeID typeID = compiler.get_type(resource.base_type_id).member_types[i];
                                spirv_cross::SPIRType SPIRtype = compiler.get_type(compiler.get_type(resource.base_type_id).member_types[i]);

                                uint32_t count = 1;
                                if (SPIRtype.array.size() == 1) {
                                    count = SPIRtype.array[0];
                                }
                                else if(SPIRtype.array.size() == 0) {
                                    count = 1;
                                }

                                uint32_t memberCount = compiler.get_type(typeID).member_types.size();
                                for (uint32_t j = 0; j < memberCount; j++) {
                                    const auto& name = compiler.get_member_name(typeID, j);
                                    UniformType UniformType = SPIRVTypeToShaderUniformType(compiler.get_type(compiler.get_type(typeID).member_types[j]));
                                    uint32_t size = compiler.get_declared_struct_member_size(compiler.get_type(typeID), j);
                                    uint32_t offset = compiler.type_struct_member_offset(compiler.get_type(typeID), j);

                                    sstruct.AddUniform(ShaderUniform(name, UniformType, size, offset));
                                }
                                buffer.Uniforms.push_back(ShaderUniform(name, &sstruct, count));
                            }
                            else {
                                buffer.Uniforms.push_back(ShaderUniform(name, type, size, offset));
                            }
                                
                                
                        }

                        // move uniform buffers to their own class
                        glGenBuffers(1, &buffer.RendererID);
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


                    

                    
                    m_Resources[name] = ShaderResource(name, glShader->GetType(), Render::ResourceType::Texture2D, binding, arrayCount);
                    if (arrayCount > 1)
                        SetUniformIntArray(name, samplers, arrayCount);
                    else if (arrayCount == 1)
                        SetUniform(name, binding);

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

        void OpenGLPipeline::SetUniform(const std::string& name, uint32_t value) {
            glUniform1ui(GetUniformLocation(name), value);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, float value) {
            glUniform1f(GetUniformLocation(name), value);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const glm::vec2& value) {
            glUniform2f(GetUniformLocation(name), value.x, value.y);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const glm::vec3& value) {
            glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
        }

        void OpenGLPipeline::SetUniform(const std::string& name, const glm::vec4& value) {
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

        void OpenGLPipeline::UploadUniformFloat2(uint32_t location, const glm::vec2& value) {
            glUniform2f(location, value.x, value.y);
        }

        void OpenGLPipeline::UploadUniformFloat3(uint32_t location, const glm::vec3& value) {
            glUniform3f(location, value.x, value.y, value.z);
        }

        void OpenGLPipeline::UploadUniformFloat4(uint32_t location, const glm::vec4& value) {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }

        void OpenGLPipeline::UploadUniformIntArray(uint32_t location, int32_t* values, uint32_t count) {
            glUniform1iv(location, count, values);
        }
    }
}