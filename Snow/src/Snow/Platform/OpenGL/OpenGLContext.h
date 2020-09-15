#pragma once

#include "Snow/Render/RenderContext.h"

#include "Snow/Platform/OpenGL/OpenGLSwapChain.h"

namespace Snow {
    namespace Render {
        class OpenGLContext : public Context {
        public:
            OpenGLContext(const ContextSpecification& spec);

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }

            OpenGLSwapChain& GetSwapChain() { return *m_OpenGLSwapChain; }
        private:
            ContextSpecification m_Specification;

            OpenGLSwapChain* m_OpenGLSwapChain;
        };
    }
}