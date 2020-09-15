#include <spch.h>
#include "Snow/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Snow {
    namespace Render {
        OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) {

        }

        void OpenGLVertexBuffer::Bind() const {

        }

        void OpenGLVertexBuffer::Unbind() const {

        }

        OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size) :
            m_Count(size / sizeof(uint32_t)) {

        }

        void OpenGLIndexBuffer::Bind() const {

        }

        void OpenGLIndexBuffer::Unbind() const {

        }
    }
}