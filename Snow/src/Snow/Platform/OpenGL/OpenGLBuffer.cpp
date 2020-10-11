#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {
        OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) {

            m_LocalBuffer = Buffer(data, size);

            glGenBuffers(1, &m_RendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, size, (void*)data, GL_STATIC_DRAW);
        }

        void OpenGLVertexBuffer::Bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        }

        void OpenGLVertexBuffer::Unbind() const {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void OpenGLVertexBuffer::SetData(void* data, uint32_t size) {

            if (m_Size != size) {
                m_Size = size;
                Buffer buffercopy = Buffer::Copy(m_LocalBuffer);
                m_LocalBuffer.Allocate(size);
                m_LocalBuffer.Write(buffercopy.Data, buffercopy.Size, 0);
                
            }
            else
                m_LocalBuffer.Write(data, size, 0);

            glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
            glBufferData(GL_ARRAY_BUFFER, size, (void*)data, GL_STATIC_DRAW);
        }

        OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size) :
            m_Count(size / sizeof(uint32_t)) {

            glGenBuffers(1, &m_RendererID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (void*)data, GL_STATIC_DRAW);
        }

        void OpenGLIndexBuffer::Bind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        }

        void OpenGLIndexBuffer::Unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
}