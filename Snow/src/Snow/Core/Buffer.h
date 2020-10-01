#pragma once

#include "Base.h"
#include <memory>

namespace Snow {
    using byte = unsigned char;
    struct Buffer {
        byte* Data;
        uint32_t Size;

        Buffer() :
            Data(nullptr), Size(0) {}

        Buffer(byte* data, uint32_t size) :
            Data(data), Size(size) {}

        static Buffer Copy(void* data, uint32_t size) {
            Buffer buffer;
            buffer.Allocate(size);
            memcpy(buffer.Data, data, size);
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

        void ZeroInitialize() {
            if(Data)
                memset(Data, 0, Size);
        }

        template<typename T>
        T& Read(uint32_t offset = 0) {
            return *(T*)(Data + offset);
        }

        void Write(void* data, uint32_t size, uint32_t offset = 0) {
            //assert(offset + size <= this->Size);
            SNOW_CORE_TRACE("Buffer stats, Data mem {0}, data mem {1}, Size {2}, size {3}", (uint8_t*)Data, (uint8_t*)data, Size, size);
            memcpy(Data + offset, data, size);
        }

        template<typename T>
        T* As() {
            return (T*)Data;
        }

        operator bool() const {
            return Data;
        }

        byte& operator[](int index) {
            return Data[index];
        }

        byte operator[](int index) const {
            return Data[index];
        }

        inline uint32_t GetSize() const { return Size; } 
    };
}