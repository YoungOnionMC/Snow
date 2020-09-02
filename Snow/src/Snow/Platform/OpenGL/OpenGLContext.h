#pragma once

#include "Snow/Render/RenderContext.h"

namespace Snow {
    namespace Render {
        class OpenGLContext : public Context {
        public:
            OpenGLContext(const ContextSpecification& spec);

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }
        private:
            ContextSpecification m_Specification;
        };
    }
}