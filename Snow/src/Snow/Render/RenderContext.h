#pragma once

namespace Snow {
    namespace Render {


        struct ContextSpecification {
            void* WindowHandle;
        };

        class Context : public RefCounted {
        public:

            ~Context() = default;

            virtual void Create() = 0;
            virtual void BeginFrame() = 0;
            virtual void SwapBuffers() = 0;

            virtual const ContextSpecification& GetSpecification() const = 0;

            static Ref<Context> Create(const ContextSpecification& spec);
        private:
            static bool s_Created;
        };
    }
}