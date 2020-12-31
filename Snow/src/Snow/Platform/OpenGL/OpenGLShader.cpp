#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLShader.h"

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
            }
        }

        OpenGLShader::OpenGLShader(ShaderType type, const std::string& path) :
            m_Path(path), m_Type(type) {

            size_t found = path.find_last_of("/\\");
            m_Name = found != std::string::npos ? path.substr(found + 1) : path;
            found = m_Name.find_last_of(".");
            m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;


            m_Source = ReadShaderFromFile(m_Path);
            CreateSPIRVBinaryCache();
            CreateGLSLBinaryCache();
            CreateOpenGLShaderModule(false);
            GLSLReflect();
            //Load(m_Source);

        }

        void OpenGLShader::Load(const std::string& source) {
            m_RendererID = glCreateShader(GetShaderType(m_Type));
            const GLchar* sourceC = (const GLchar*)m_Source.c_str();
            glShaderSource(m_RendererID, 1, &sourceC, 0);

            glCompileShader(m_RendererID);

            GLint isCompiled = 0;
            glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &isCompiled);
            if(isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
                SNOW_CORE_ERROR("Shader compilation failed {0}, Shader Path {1}", &infoLog[0], m_Path);
                glDeleteShader(m_RendererID);
            }
        }

        

        void OpenGLShader::CreateSPIRVBinaryCache() {
            std::filesystem::path directoryPath = m_Path;
            if (!std::filesystem::is_directory(directoryPath.parent_path() / "cached")) {
                std::string dirPath = (directoryPath.parent_path() / "cached").string();
                std::filesystem::create_directory(directoryPath.parent_path() / "cached");
            }
            {
                std::filesystem::path filePath = m_Path;
                auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                std::string cachedFilePath = path.string();

                FILE* f = fopen(cachedFilePath.c_str(), "rb");
                if (f) {
                    fseek(f, 0, SEEK_END);
                    uint64_t size = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    m_SPIRVBinaryData = std::vector<uint32_t>(size / sizeof(uint32_t));
                    fread(m_SPIRVBinaryData.data(), sizeof(uint32_t), m_SPIRVBinaryData.size(), f);
                    fclose(f);
                }
            }

            if (m_SPIRVBinaryData.size() == 0) {
                shaderc::Compiler compiler;
                shaderc::CompileOptions options;
                options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
                options.SetOptimizationLevel(shaderc_optimization_level_zero);

                const bool optimize = false;
                if (optimize)
                    options.SetOptimizationLevel(shaderc_optimization_level_performance);



                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(m_Source, SnowShaderTypeToShaderC(m_Type), m_Path.c_str(), options);

                if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    SNOW_CORE_ERROR(module.GetErrorMessage());
                }

                m_SPIRVBinaryData = std::vector<uint32_t>(module.cbegin(), module.cend());
                
                {
                    std::filesystem::path filePath = m_Path;
                    auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_vulkan");
                    std::string cachedFilePath = path.string();

                    FILE* f = fopen(cachedFilePath.c_str(), "wb");
                    fwrite(m_SPIRVBinaryData.data(), sizeof(uint32_t), m_SPIRVBinaryData.size(), f);
                    fclose(f);
                }

                //SNOW_CORE_TRACE("Size of shader {0}", m_SPIRVBinaryData.size());

                
            }
        }

        void OpenGLShader::CreateGLSLBinaryCache() {
            {
                std::filesystem::path filePath = m_Path;
                auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_glsl");
                std::string cachedFilePath = path.string();

                FILE* f = fopen(cachedFilePath.c_str(), "rb");
                if (f) {
                    fseek(f, 0, SEEK_END);
                    uint64_t size = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    m_GLSLBinaryData = std::vector<uint32_t>(size / sizeof(uint32_t));
                    fread(m_GLSLBinaryData.data(), sizeof(uint32_t), m_GLSLBinaryData.size(), f);
                    fclose(f);
                }
            }

            if (m_GLSLBinaryData.size() == 0) {
                shaderc::Compiler compiler;
                shaderc::CompileOptions options;
                options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);

                spirv_cross::CompilerGLSL compilerGLSL(m_SPIRVBinaryData);

                std::string source = compilerGLSL.compile();
                m_GLSLSourceData = SplitString(source, "\r\n");

                printf("====================\n");
                printf("Shader:\n%s\n", source.data());
                printf("====================\n");

                

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, SnowShaderTypeToShaderC(m_Type), m_Path.c_str(), options);

                if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                    SNOW_CORE_ERROR(module.GetErrorMessage());
                }

                m_GLSLBinaryData = std::vector<uint32_t>(module.begin(), module.end());

                {
                    std::filesystem::path filePath = m_Path;
                    auto path = filePath.parent_path() / "cached" / (filePath.filename().string() + ".cached_glsl");
                    std::string cachedFilePath = path.string();

                    FILE* f = fopen(cachedFilePath.c_str(), "wb");
                    fwrite(m_GLSLBinaryData.data(), sizeof(uint32_t), m_GLSLBinaryData.size(), f);
                    fclose(f);
                }
            }
        }

        void OpenGLShader::GLSLReflect() {
            shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment(shaderc_target_env_opengl_compat, shaderc_env_version_opengl_4_5);

            spirv_cross::CompilerGLSL compilerGLSL(m_SPIRVBinaryData);

            std::string source = compilerGLSL.compile();
            m_GLSLSourceData = SplitString(source, "\r\n");

            shaderc::PreprocessedSourceCompilationResult reult = compiler.PreprocessGlsl(source, SnowShaderTypeToShaderC(m_Type), m_Path.c_str(), options);
            m_GLSLBinaryData = std::vector<uint32_t>(reult.begin(), reult.end());

            

            //glslang::TShader glslShader()
        }

        void OpenGLShader::CreateOpenGLShaderModule(bool spirvModule) {
            m_RendererID = glCreateShader(GetShaderType(m_Type));

            if (spirvModule) {
                glShaderBinary(1, &m_RendererID, GL_SHADER_BINARY_FORMAT_SPIR_V, m_GLSLSourceData.data(), m_GLSLSourceData.size() * sizeof(uint32_t));
                glSpecializeShader(m_RendererID, "main", 0, nullptr, nullptr);

                glCompileShader(m_RendererID);
                GLint isCompiled = 0;
                glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE) {
                    GLint maxLength = 0;
                    glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
                    SNOW_CORE_ERROR("Shader compilation failed {0}, Shader Path {1}", &infoLog[0], m_Path);
                    glDeleteShader(m_RendererID);
                }

                SNOW_CORE_TRACE("Created SPIRV module for OpenGL Shader {0}", m_Path);
                SNOW_CORE_TRACE("   size in bytes: {0}", m_GLSLBinaryData.size() * sizeof(uint32_t));
            }
            else {
                const GLchar* source = (const GLchar*)m_Source.c_str();

                glShaderSource(m_RendererID, 1, &source, 0);

                glCompileShader(m_RendererID);
                GLint isCompiled = 0;
                glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE) {
                    GLint maxLength = 0;
                    glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
                    SNOW_CORE_ERROR("Shader compilation failed {0}, Shader Path {1}", &infoLog[0], m_Path);
                    glDeleteShader(m_RendererID);
                }
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
    }
}