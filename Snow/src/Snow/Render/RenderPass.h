#pragma once

#include "Snow/Core/Ref.h"

namespace Snow {
    namespace Render {
        struct RenderPassSpecification {

        };

        class RenderPass : public RefCounted {
        public:

            static Ref<RenderPass> Create(const RenderPassSpecification& spec);

        };
    }
}