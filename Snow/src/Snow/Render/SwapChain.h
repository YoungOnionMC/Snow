#pragma once

#include "Snow/Core/Ref.h"

namespace Snow {
    namespace Render {
        struct SwapChainSpecification {
            uint32_t Width, Height;
            bool VSync;
        };

        class SwapChain : public RefCounted {
        public:

            virtual void Init() = 0;
            virtual void InitSurface(void* windowHandle) = 0;
            virtual void SwapBuffers() = 0;

            virtual void Create(const SwapChainSpecification& spec) = 0;

            virtual void OnResize(uint32_t width, uint32_t height) = 0;

            virtual void BeginFrame() = 0;

            virtual uint32_t GetCurrentBufferIndex() = 0;

            static SwapChain* CreateSwapChain(const SwapChainSpecification& spec);
            
        };
    }
}