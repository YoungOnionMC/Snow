#pragma once

#include "Snow/Core/Base.h"
#include "Snow/Core/Log.h"
#include <memory>

namespace Snow {
    using byte = unsigned char;
    struct Buffer {
        void* Data;
        uint32_t Size;

        Buffer() :
            Data(nullptr), Size(0) {}

        Buffer(uint32_t size) :
            Data(nullptr), Size(size) {
            Allocate(size);
        }

        Buffer(void* data, uint32_t size) :
            Data(data), Size(size) {}

        Buffer(byte* data, uint32_t size) :
            Data(data), Size(size) {}

        static Buffer Copy(void* data, uint32_t size) {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.Data, data, size);
            return buffer;
        }

        /* Creates a new buffer out of an existing buffer
           @param[in] Buffer to copy
           @param[out] Buffer created through copy
        */
        static Buffer Copy(Buffer other) {
            Buffer buffer;
            buffer.Allocate(other.Size);
            memcpy(buffer.Data, other.Data, other.Size);
            return buffer;
        }

        void Allocate(uint32_t size) {
            delete[] Data;
            Data = nullptr;

            if(size == 0)
                return;
            
            Data = new byte[size];
            Size = size;
        }
        
        void Release() {
            delete[] Data;
            Data = nullptr;
            Size = 0;
        }

        void ZeroInitialize() {
            if(Data)
                memset(Data, 0, Size);
        }

        template<typename T>
        T& Read(uint32_t offset = 0) {
            return *(T*)((byte*)Data + offset);
        }

        byte* ReadBytes(uint32_t size, uint32_t offset) {
            SNOW_CORE_ASSERT(offset + size <= Size, "Buffer Overflow!");
            byte* buffer = new byte[size];
            memcpy(buffer, (byte*)Data + offset, size);
            return buffer;
        }

        void Write(void* data, uint32_t size, uint32_t offset = 0) {
            //SNOW_ASSERT(offset + size <= this->Size, "Trying to write outside of buffer memory");
            memcpy((byte*)Data + offset, data, size);
        }

        template<typename T>
        T* As() {
            return (T*)Data;
        }

        operator bool() const {
            return Data;
        }

        byte& operator[](int index) {
            return ((byte*)Data)[index];
        }

        byte operator[](int index) const {
            return ((byte*)Data)[index];
        }

        inline uint32_t GetSize() const { return Size; } 
    };
}