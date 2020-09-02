#pragma once
#include "Snow/Render/RenderContext.h"

namespace Snow {
    namespace Render {
        class Renderer {
        public:
            static void Init();




        private:
            static Context* s_Context;
        };
    }
}