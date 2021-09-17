#pragma once
#include <spch.h>

#include "Snow/Core/Ref.h"

namespace Snow {
    namespace Render {
        class VertexBuffer : public RefCounted {
        public:
            virtual ~VertexBuffer() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) = 0;
            virtual void RTSetData(void* data, uint32_t size, uint32_t offset = 0) = 0;

            static Ref<VertexBuffer> Create(uint32_t size = 0);
            static Ref<VertexBuffer> Create(void* data = nullptr, uint32_t size = 0);
        };



        class IndexBuffer : public RefCounted {
        public:
            virtual ~IndexBuffer() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) = 0;
            virtual void RTSetData(void* data, uint32_t size, uint32_t offset = 0) = 0;

            virtual uint32_t GetCount() const = 0;

            static Ref<IndexBuffer> Create(void* data = nullptr, uint32_t size = 0);
        };
    }
}