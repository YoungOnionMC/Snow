#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLPipeline.h"

#include "Snow/Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {

        OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec) :
            m_Specification(spec) {

            glGenVertexArrays(1, &m_PipelineVertexArrayHandle);
            glBindVertexArray(m_PipelineVertexArrayHandle);

            LinkShaders();
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

    }
}