#pragma once
#include "Snow/Render/Buffer.h"

#include "Snow/Core/Buffer.h"

namespace Snow {
    namespace Render {
        class OpenGLVertexBuffer : public VertexBuffer {
        public:
            OpenGLVertexBuffer(void* data, uint32_t size);

            void Bind() const override;
            void Unbind() const override;

            void SetData(void* data, uint32_t size) override;

        private:
            uint32_t m_RendererID;

            Buffer m_LocalBuffer;
            uint32_t m_Size;
        };

        class OpenGLIndexBuffer : public IndexBuffer {
        public:
            OpenGLIndexBuffer(void* data, uint32_t size);

            void Bind() const override;
            void Unbind() const override;

            void SetData(void* data, uint32_t size) override;

            virtual uint32_t GetCount() const override { return m_Count; }

        private:
            uint32_t m_RendererID;

            Buffer m_LocalBuffer;
            uint32_t m_Count;
        };
    }
}