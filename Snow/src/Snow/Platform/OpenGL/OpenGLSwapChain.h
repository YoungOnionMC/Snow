#pragma once
#include "Snow/Render/SwapChain.h"

namespace Snow {
    namespace Render {
        class OpenGLSwapChain : public SwapChain {
        public:
            OpenGLSwapChain(const SwapChainSpecification& spec);

            virtual void SwapBuffers() override;
        private:
            SwapChainSpecification m_Specification;
        };
    }
}