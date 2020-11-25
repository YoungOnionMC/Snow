#pragma once

namespace Snow {
    namespace Render {

        enum class RenderAPIType {
            None = 0,
            OpenGL, DirectX, Vulkan
        };

        struct ContextSpecification {
            static RenderAPIType s_RenderAPIType;
            void* WindowHandle;
        };

        class Context {
        public:

            ~Context() = default;

            virtual const ContextSpecification& GetSpecification() const = 0;

            static Context* Create(const ContextSpecification& spec);
        private:
            static bool s_Created;
        };
    }
}