#pragma once
#include <spch.h>

namespace Snow {
    namespace Render {
        namespace API {
            class VertexBuffer {
            public:
                virtual ~VertexBuffer() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                static VertexBuffer* Create(void* data = nullptr, uint32_t size = 0);
            };



            class IndexBuffer {
            public:
                virtual ~IndexBuffer() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                virtual uint32_t GetCount() const = 0;

                static IndexBuffer* Create(void* data = nullptr, uint32_t size = 0);
            };
        }
    }
}