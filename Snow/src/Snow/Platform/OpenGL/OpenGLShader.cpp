#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLShader.h"
#include "Snow/Platform/OpenGL/OpenGLCommon.h"

#include <glad/glad.h>

#include <shaderc.hpp>
#include <spirv_glsl.hpp>
#include <glslang/Public/ShaderLang.h>

#include <filesystem>

namespace Snow {
    namespace Render {
        std::vector<std::string> SplitString(const std::string& string, const char* delimiter) {
            std::vector<std::string> result;
            std::string::size_type pos = 0;
            std::string::size_type prev = 0;
            while ((pos = string.find('\n', prev)) != std::string::npos) {
                result.push_back(string.substr(prev, pos - prev));
                prev = pos + 1;
            }
            //result.push_back(string.substr(prev));
            return result;
        }

        shaderc_shader_kind OpenGLShader::SnowShaderTypeToShaderC(ShaderType type) {
            switch(type){
                case ShaderType::Vertex:    return shaderc_shader_kind::shaderc_glsl_vertex_shader;
                case ShaderType::Pixel:     return shaderc_shader_kind::shaderc_glsl_fragment_shader;
                case ShaderType::Compute:   return shaderc_shader_kind::shaderc_compute_shader;
            }
        }

        OpenGLShader::OpenGLShader(const std::initializer_list<ShaderModule>& shaderModules) :
            m_ShaderModules(shaderModules) {
           
            for (auto [type, path] : m_ShaderModules) {
                m_Paths.push_back(path);
                size_t found = path.find_last_of("/\\");
                m_Name = found != std::string::npos ? path.substr(found + 1) : path;
                found = m_Name.find_first_of(".");
                m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;
                
            }
            
            Reload();
        }

        void OpenGLShader::Bind() const {
            GLCheckError(glUseProgram(m_RendererID));
        }

        void OpenGLShader::Reload() {
            if (m_ShaderIDs.size()) {
                for (auto id : m_ShaderIDs) {
                    GLCheckError(glDeleteShader(id));
                }
            }
            if (m_RendererID)
                GLCheckError(glDeleteProgram(m_RendererID));

            m_RendererID = glCreateProgram();

            if (m_ShaderModules.size() > 1) {
                m_ShaderSources.resize(m_ShaderModules.size());
                m_ShaderTypes.resize(m_ShaderModules.size());
                for (uint32_t i = 0; i < m_ShaderModules.size(); i++) {
                    //auto& [type, path] : m_ShaderModules
                    m_ShaderSources[i] = ReadShaderFromFile(m_ShaderModules[i].second);
                    SNOW_CORE_ASSERT(m_ShaderModules[i].first != ShaderType::None);
                    m_ShaderTypes[i] = m_ShaderModules[i].first;

                }
                Load();
            }
            else if (m_ShaderModules.size() == 1) {
                SNOW_CORE_ASSERT(m_ShaderModules[0].second != "");
                const auto& source = ReadShaderFromFile(m_ShaderModules[0].second);
                auto shaderModules = PreProcess(source);
                m_ShaderSources = shaderModules.first;
                m_ShaderTypes = shaderModules.second;

                std::string originalPath = m_Paths[0];
                m_Paths.resize(m_ShaderSources.size());
                size_t found = originalPath.find_last_of("/\\");
                std::string parentPath = found != std::string::npos ? originalPath.substr(0, found + 1) : originalPath;
                for (uint32_t i = 0; i < m_ShaderSources.size(); i++)
                    m_Paths[i] = std::string(parentPath + m_Name + "." + ShaderTypeToString(m_ShaderTypes[i]) + ".glsl");

                Load();
            }
            else {
                SNOW_CORE_ERROR("Shader failed to reload!");
            }

            
        }

        void OpenGLShader::Load() {
            m_SPIRVBinaryData.resize(m_ShaderSources.size());
            m_GLSLBinaryData.resize(m_ShaderSources.size());
            m_GLSLSourceData.resize(m_ShaderSources.size());

            m_ShaderIDs.resize(m_ShaderSources.size());

            for (uint32_t i = 0; i < m_ShaderSources.size(); i++) {
                bool isCompute = false;
                if (m_ShaderTypes[i] == ShaderType::Compute) {
                    isCompute = true;
                }
                else {
                    CreateSPIRVBinaryCache(i);
                    CreateGLSLBinaryCache(i);
                }
                CreateOpenGLShaderModule(i, !isCompute);
            }
            //GLSLReflect();

            LinkShaders();

            for (uint32_t i = 0; i < m_ShaderSources.size(); i++) {
                SNOW_CORE_ASSERT(m_GLSLBinaryData.size() == m_ShaderTypes.size(), "Shader data does not match their types");
                auto& data = m_GLSLBinaryData[i];
                auto type = m_ShaderTypes[i];
                if (type != ShaderType::Compute)
                    SPIRVReflection(data, type);
            }
        }

        const ShaderUniformBuffer& OpenGLShader::GetUniformBuffer(const std::string& name) const {
            for (auto& [bindingPoint, ub] : m_UniformBuffers) {
                if (ub.Name == name) {
                    return ub;
                }
            }
            SNOW_CORE_WARN("Could not find uniform buffer, returning UBO 0");
            return m_UniformBuffers[0];
        }

        void OpenGLShader::SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size) {
            uint8_t* buffer = new uint8_t[size];
            memcpy(buffer, data, size);

            ShaderUniformBuffer* uniformBuffer = nullptr;
            for (auto& [bindingPoint, ub] : m_UniformBuffers) {
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

        void OpenGLShader::SetUniformBufferData(uint32_t bindingPoint, void* data, uint32_t size) {
            uint8_t* buffer = new uint8_t[size];
            memcpy(buffer, data, size);


            ShaderUniformBuffer* uniformBuffer = &m_UniformBuffers[bindingPoint];

            glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->RendererID);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, buffer);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            delete[] buffer;
            //glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        OpenGLShader::ShaderModulePair OpenGLShader::PreProcess(const std::string& source) {
            ShaderModulePair shaderModules;
            const char* typeToken = "#type";
            size_t typeTokenLength = strlen(typeToken);
            size_t pos = source.find(typeToken, 0);
            while (pos != std::string::npos) {
                size_t eol = source.find_first_of("\r\n", pos);
                SNOW_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
                size_t begin = pos + typeTokenLength + 1;
                std::string type = source.substr(begin, eol - begin);
                SNOW_CORE_ASSERT(type == "vertex" || type == "pixel" || type == "fragment" || type == "compute" || type == "geometry", "Invalid shader type");

                size_t nextLinePos = source.find_first_not_of("\r\n", eol);
                pos = source.find(typeToken, nextLinePos);
                auto shaderType = ShaderTypeFromString(type);
                
                shaderModules.first.push_back(source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)));
                shaderModules.second.push_back(shaderType);
                
            }

            return shaderModules;
            
        }

        void OpenGLShader::CreateSPIRVBinaryCache(uint32_t shaderIndex) {
            {
                std::filesystem::path directoryPath = m_Paths[shaderIndex];
                if (!std::filesystem::is_directory(directoryPath.parent_path() / "cached")) {
                    std::string dirPath = (directoryPath.parent_path() / "cached").string();
                    std::filesystem::create_directory(directoryPath.parent_path() / "cached");
                }
                {
                    std::filesystem::path filePath = m_Paths[shaderIndex];
                    auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                    std::string cachedFilePath = path.string();

                    FILE* f = fopen(cachedFilePath.c_str(), "rb");
                    if (f) {
                        fseek(f, 0, SEEK_END);
                        uint64_t size = ftell(f);
                        fseek(f, 0, SEEK_SET);
                        m_SPIRVBinaryData[shaderIndex] = std::vector<uint32_t>(size / sizeof(uint32_t));
                        fread(m_SPIRVBinaryData[shaderIndex].data(), sizeof(uint32_t), m_SPIRVBinaryData[shaderIndex].size(), f);
                        fclose(f);
                    }
                }
            }

            if (m_SPIRVBinaryData[shaderIndex].size() == 0) {
                shaderc::Compiler compiler;
                shaderc::CompileOptions options;
                options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
                options.SetOptimizationLevel(shaderc_optimization_level_zero);

                const bool optimize = false;
                if (optimize)
                    options.SetOptimizationLevel(shaderc_optimization_level_performance);

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(m_ShaderSources[shaderIndex], SnowShaderTypeToShaderC(m_ShaderTypes[shaderIndex]), m_Paths[shaderIndex].c_str(), options);

                if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    SNOW_CORE_ERROR(module.GetErrorMessage());
                    SNOW_CORE_ASSERT(false);
                }

                m_SPIRVBinaryData[shaderIndex] = std::vector<uint32_t>(module.cbegin(), module.cend());

                SNOW_CORE_TRACE("Created SPIRV module for OpenGL Shader {0}", m_Paths[shaderIndex]);
                SNOW_CORE_TRACE("   size in bytes: {0}", m_SPIRVBinaryData[shaderIndex].size());
                
                {
                    std::filesystem::path filePath = m_Paths[shaderIndex];
                    auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                    std::string cachedFilePath = path.string();

                    FILE* f = fopen(cachedFilePath.c_str(), "wb");
                    fwrite(m_SPIRVBinaryData[shaderIndex].data(), sizeof(uint32_t), m_SPIRVBinaryData[shaderIndex].size(), f);
                    fclose(f);
                }
            }
        }

        void OpenGLShader::CreateGLSLBinaryCache(uint32_t shaderIndex) {

            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);

            spirv_cross::CompilerGLSL glslCompiler(m_SPIRVBinaryData[shaderIndex]);

            std::filesystem::path filePath = m_Paths[shaderIndex];
            auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_glsl");
            std::string cachedFilePath = path.string();

            FILE* f = fopen(cachedFilePath.c_str(), "rb");
            if (f) {
                fseek(f, 0, SEEK_END);
                uint64_t size = ftell(f);
                fseek(f, 0, SEEK_SET);
                m_GLSLBinaryData[shaderIndex] = std::vector<uint32_t>(size / sizeof(uint32_t));
                fread(m_GLSLBinaryData[shaderIndex].data(), sizeof(uint32_t), m_GLSLBinaryData[shaderIndex].size(), f);
                fclose(f);
            }
            if(m_GLSLBinaryData[shaderIndex].size() == 0) {
                m_GLSLSourceData[shaderIndex] = SplitString(m_ShaderSources[shaderIndex], "\r\n");

                auto glslCompiledString = glslCompiler.compile();

                printf("====================\n");
                //printf("Shader:\n%s\n", m_ShaderSources[shaderIndex].c_str());
                printf("Shader:\n%s\n", glslCompiledString.c_str());
                printf("====================\n");

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(glslCompiledString, SnowShaderTypeToShaderC(m_ShaderTypes[shaderIndex]), m_Paths[shaderIndex].c_str(), options);
                // This is used to compile from SPIR-V back into a glsl module, which is kinda pointless
                //shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, SnowShaderTypeToShaderC(m_ShaderTypes[shaderIndex]), m_Paths[shaderIndex].c_str(), options);

                if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    SNOW_CORE_ERROR(module.GetErrorMessage());
                    SNOW_CORE_ASSERT(false);
                }

                m_GLSLBinaryData[shaderIndex] = std::vector<uint32_t>(module.begin(), module.end());

                SNOW_CORE_TRACE("Created GLSL module for OpenGL Shader {0}", m_Paths[shaderIndex]);
                SNOW_CORE_TRACE("   size in bytes: {0}", m_GLSLBinaryData[shaderIndex].size());

                {
                    std::filesystem::path filePath = m_Paths[shaderIndex];
                    auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_glsl");
                    std::string cachedFilePath = path.string();

                    FILE* f = fopen(cachedFilePath.c_str(), "wb");
                    fwrite(m_GLSLBinaryData[shaderIndex].data(), sizeof(uint32_t), m_GLSLBinaryData[shaderIndex].size(), f);
                    fclose(f);
                }

            }
        }

        void OpenGLShader::GLSLReflect() {
            
        }

        void OpenGLShader::CreateOpenGLShaderModule(uint32_t shaderIndex, bool useSPIRVModule) {
            m_ShaderIDs[shaderIndex] = glCreateShader(GetShaderType(m_ShaderTypes[shaderIndex]));

            if (useSPIRVModule) {
                GLCheckError(glShaderBinary(1, &m_ShaderIDs[shaderIndex], GL_SHADER_BINARY_FORMAT_SPIR_V, m_GLSLBinaryData[shaderIndex].data(), m_GLSLBinaryData[shaderIndex].size() * sizeof(uint32_t)));
                GLCheckError(glSpecializeShader(m_ShaderIDs[shaderIndex], "main", 0, nullptr, nullptr));
                

                glCompileShader(m_ShaderIDs[shaderIndex]);
                GLint isCompiled = 0;
                glGetShaderiv(m_ShaderIDs[shaderIndex], GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE) {
                    GLint maxLength = 0;
                    glGetShaderiv(m_ShaderIDs[shaderIndex], GL_INFO_LOG_LENGTH, &maxLength);

                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(m_ShaderIDs[shaderIndex], maxLength, &maxLength, &infoLog[0]);
                    SNOW_CORE_ERROR("Shader compilation failed {0}, Shader Path {1}, Shader Type {2}", &infoLog[0], m_Paths[shaderIndex], m_ShaderTypes[shaderIndex]);
                    glDeleteShader(m_ShaderIDs[shaderIndex]);

                    SNOW_CORE_ASSERT(false);
                }
            }
            else {
                const GLchar* source = (const GLchar*)m_ShaderSources[shaderIndex].c_str();

                glShaderSource(m_ShaderIDs[shaderIndex], 1, &source, 0);

                glCompileShader(m_ShaderIDs[shaderIndex]);
                GLint isCompiled = 0;
                glGetShaderiv(m_ShaderIDs[shaderIndex], GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE) {
                    GLint maxLength = 0;
                    glGetShaderiv(m_ShaderIDs[shaderIndex], GL_INFO_LOG_LENGTH, &maxLength);

                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(m_ShaderIDs[shaderIndex], maxLength, &maxLength, &infoLog[0]);
                    SNOW_CORE_ERROR("Shader compilation failed {0}, Shader Path {1}, Shader Type {2}", &infoLog[0], m_Paths[shaderIndex], m_ShaderTypes[shaderIndex]);
                    glDeleteShader(m_ShaderIDs[shaderIndex]);
                }
            }

            glAttachShader(m_RendererID, m_ShaderIDs[shaderIndex]);

            SNOW_CORE_TRACE("Created and attached shader {0} to the current program", m_Paths[shaderIndex]);
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

        void OpenGLShader::LinkShaders() {
            GLCheckError(glLinkProgram(m_RendererID));
            GLint isLinked = 0;
            GLCheckError(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked));
            if (isLinked == GL_FALSE) {
                GLint maxLength = 0;
                glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
                SNOW_CORE_ERROR("Program linking failed:\n{0}", &infoLog[0]);

                glDeleteProgram(m_RendererID);

                for (auto shaderID : m_ShaderIDs) {
                    GLCheckError(glDeleteShader(shaderID));
                    GLCheckError(glDetachShader(m_RendererID, shaderID));
                }
                return;
            }
            for (auto shaderID : m_ShaderIDs) {
                GLCheckError(glDetachShader(m_RendererID, shaderID));
            }

            SNOW_CORE_TRACE("Shader program created successfully");
        }

        void OpenGLShader::SPIRVReflection(std::vector<uint32_t>& data, ShaderType type) {
            GLCheckError(glUseProgram(m_RendererID));

            std::vector<spirv_cross::SPIRType> outputAttributes;


            spirv_cross::Compiler compiler(data);
            spirv_cross::ShaderResources res = compiler.get_shader_resources();

            SNOW_CORE_INFO("==========================");
            SNOW_CORE_INFO("OpenGL::SPIRVReflect -  {0}", m_Paths[0]);
            SNOW_CORE_INFO("    {0} Inputs Attributes", res.stage_inputs.size());
            SNOW_CORE_INFO("    {0} Outputs Attributes", res.stage_outputs.size());
            SNOW_CORE_INFO("    {0} Uniform buffers", res.uniform_buffers.size());
            SNOW_CORE_INFO("    {0} Resources", res.sampled_images.size());

            uint32_t inputAttribIndex = 0;
            /*
            if (m_ShaderTypes[shaderTypeIndex] == ShaderType::Vertex) {
                // Layout testing does not work for now

                
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
            */
            

            uint32_t outputAttribIndex = 0;
            for (const auto& outputAttrib : res.stage_outputs) {
                auto attribType = compiler.get_type(outputAttrib.type_id);
                auto binding = compiler.get_decoration(outputAttrib.id, spv::DecorationBinding);
                auto location = compiler.get_decoration(outputAttrib.id, spv::DecorationLocation);
                auto& name = outputAttrib.name;

                SNOW_CORE_TRACE("Found output {0} - binding: {1}, location: {2}", name, binding, location);
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
            for (const spirv_cross::Resource& resource : res.uniform_buffers) {
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
                            else if (SPIRtype.array.size() == 0) {
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
            for (const spirv_cross::Resource& resource : res.sampled_images) {
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





                m_Resources[name] = ShaderResource(name, type, Render::ResourceType::Texture2D, binding, arrayCount);
                if (arrayCount > 1)
                    SetUniformIntArray(name, samplers, arrayCount);
                else if (arrayCount == 1)
                    SetUniform(name, binding);

                //SNOW_CORE_INFO("   Binding resource {0}, at location {1}", name, location);


            }

        }

        std::string OpenGLShader::ReadShaderFromFile(const std::string& path) {
            std::string result;
            std::ifstream inFile(path, std::ios::in | std::ios::binary);
            if(inFile.is_open()) {
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

        GLenum OpenGLShader::GetShaderType(ShaderType type) {
            switch(type) {
            case ShaderType::Vertex:    return GL_VERTEX_SHADER;
            case ShaderType::Pixel:    return GL_FRAGMENT_SHADER;
            case ShaderType::Compute:   return GL_COMPUTE_SHADER;
            }
        }

  

        uint32_t OpenGLShader::GetUniformLocation(const std::string& name) {
            GLint GLlocation = glGetUniformLocation(m_RendererID, name.c_str());

            if (GLlocation == -1) {
                for (auto& [uniformName, uniformLocation] : m_UniformLocations) {
                    if (name == uniformName)
                        return uniformLocation;
                }
                for (auto& [resName, resource] : m_Resources) {
                    if (name == resName)
                        return resource.GetRegister();
                }

                SNOW_CORE_ERROR("Could not find uniform {0} in pipeline", name.c_str());
            }

            return (uint32_t)GLlocation;
        }

        void OpenGLShader::SetUniform(const std::string& name, int value) {
            glUniform1i(GetUniformLocation(name), value);
        }

        void OpenGLShader::SetUniform(const std::string& name, uint32_t value) {
            glUniform1ui(GetUniformLocation(name), value);
        }

        void OpenGLShader::SetUniform(const std::string& name, float value) {
            glUniform1f(GetUniformLocation(name), value);
        }

        void OpenGLShader::SetUniform(const std::string& name, const glm::vec2& value) {
            glUniform2f(GetUniformLocation(name), value.x, value.y);
        }

        void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value) {
            glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
        }

        void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value) {
            glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
        }

        void OpenGLShader::SetUniformIntArray(const std::string& name, int* values, uint32_t count) {
            glUniform1iv(GetUniformLocation(name), count, values);
        }

        void OpenGLShader::UploadUniformInt(uint32_t location, int32_t value) {
            glUniform1i(location, value);
        }

        void OpenGLShader::UploadUniformFloat(uint32_t location, float value) {
            glUniform1f(location, value);
        }

        void OpenGLShader::UploadUniformFloat2(uint32_t location, const glm::vec2& value) {
            glUniform2f(location, value.x, value.y);
        }

        void OpenGLShader::UploadUniformFloat3(uint32_t location, const glm::vec3& value) {
            glUniform3f(location, value.x, value.y, value.z);
        }

        void OpenGLShader::UploadUniformFloat4(uint32_t location, const glm::vec4& value) {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }

        void OpenGLShader::UploadUniformIntArray(uint32_t location, int32_t* values, uint32_t count) {
            glUniform1iv(location, count, values);
        }


    }
}