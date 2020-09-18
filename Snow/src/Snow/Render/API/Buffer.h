#pragma once
#include <spch.h>

#include "Snow/Core/Ref.h"

namespace Snow {
    namespace Render {
        namespace API {
            class VertexBuffer : public RefCounted {
            public:
                virtual ~VertexBuffer() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                static Ref<VertexBuffer> Create(void* data = nullptr, uint32_t size = 0);
            };



            class IndexBuffer : public RefCounted {
            public:
                virtual ~IndexBuffer() = default;

                virtual void Bind() const = 0;
                virtual void Unbind() const = 0;

                virtual uint32_t GetCount() const = 0;

                static Ref<IndexBuffer> Create(void* data = nullptr, uint32_t size = 0);
            };
        }
    }
}