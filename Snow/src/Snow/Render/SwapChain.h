#pragma once

namespace Snow {
    namespace Render {
        struct SwapChainSpecification {

        };

        class SwapChain {
        public:

            virtual void SwapBuffers() = 0;

            static SwapChain* Create(const SwapChainSpecification& spec);
            
        };
    }
}