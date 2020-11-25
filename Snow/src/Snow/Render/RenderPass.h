#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Render/API/Framebuffer.h"

namespace Snow {
    namespace Render {
        struct RenderPassSpecification {
            Ref<Framebuffer> TargetFramebuffer;
        };

        class RenderPass : public RefCounted {
        public:
            virtual ~RenderPass() = default;

            virtual RenderPassSpecification& GetSpecification() = 0;
            virtual const RenderPassSpecification& GetSpecification() const = 0;

            static Ref<RenderPass> Create(const RenderPassSpecification& spec);

        };
    }
}