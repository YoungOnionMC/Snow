#pragma once

#include "Snow/Core/Ref.h"

namespace Snow {
	namespace Render {
		class StorageBuffer : public RefCounted {
		public:
			virtual ~StorageBuffer() {}
			virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
			virtual void RTSetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
			virtual void Resize(uint32_t size) = 0;

			virtual uint32_t GetBinding() const = 0;

			static Ref<StorageBuffer> Create(uint32_t size, uint32_t binding);
		};
	}
}