#include <spch.h>

#include "Snow/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {
        OpenGLShader::OpenGLShader(ShaderType type, const std::string& path) :
            m_Path(path), m_Type(type) {

            m_Source = ReadShaderFromFile(m_Path);
            Load(m_Source);

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