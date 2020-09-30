#pragma once
#include "Snow/Render/API/Buffer.h"

namespace Snow {
    namespace Render {
        class OpenGLVertexBuffer : public API::VertexBuffer {
        public:
            OpenGLVertexBuffer(void* data, uint32_t size);

            void Bind() const override;
            void Unbind() const override;

            void SetData(void* data, uint32_t size) override;

        private:
            uint32_t m_RendererID;
        };

        class OpenGLIndexBuffer : public API::IndexBuffer {
        public:
            OpenGLIndexBuffer(void* data, uint32_t size);

            void Bind() const override;
            void Unbind() const override;

            virtual uint32_t GetCount() const override { return m_Count; }

        private:
            uint32_t m_RendererID;

            uint32_t m_Count;
        };
    }
}