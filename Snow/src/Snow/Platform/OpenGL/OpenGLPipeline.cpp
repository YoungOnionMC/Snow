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

            
        }

        static GLenum ShaderDataTypeToOpenGLBaseType(AttribType type) {
            switch (type) {
            case AttribType::Float: return GL_FLOAT;
            case AttribType::Float2: return GL_FLOAT;
            case AttribType::Float3: return GL_FLOAT;
            case AttribType::Float4: return GL_FLOAT;
            case AttribType::Mat3x3: return GL_FLOAT;
            case AttribType::Mat4x4: return GL_FLOAT;
            case AttribType::Int:   return GL_INT;
            case AttribType::Int2:  return GL_INT;
            case AttribType::Int3:  return GL_INT;
            case AttribType::Int4:  return GL_INT;
            case AttribType::Bool: return GL_BOOL;
            }
        }

        void OpenGLPipeline::Bind() const {
            
            uint32_t attribIndex = 0;
            for(const auto& element : m_Specification.Layout) {
                auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
                glEnableVertexAttribArray(attribIndex);
                switch (glBaseType) {
                case GL_INT: {
                    glVertexAttribIPointer(attribIndex, 
                        element.GetComponentCount(), OpenGLBufferAttribType(element.Type), 
                        m_Specification.Layout.GetStride(), 
                        (const void*)(intptr_t)element.Offset);
                    break;
                }
                case GL_FLOAT: {
                    glVertexAttribPointer(attribIndex,
                        element.GetComponentCount(), OpenGLBufferAttribType(element.Type),
                        element.Normalized ? GL_TRUE : GL_FALSE,
                        m_Specification.Layout.GetStride(),
                        (const void*)(intptr_t)element.Offset);
                    break;
                }
                }
                attribIndex++;
            }
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

        

        

        


        
    }
}