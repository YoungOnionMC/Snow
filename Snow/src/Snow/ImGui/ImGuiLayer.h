#pragma once

#include "Snow/Core/Layer.h"

namespace Snow {
    class ImGuiLayer : public Core::Layer {
    public:
        virtual void BeginImGuiFrame() = 0;
        virtual void EndImGuiFrame() = 0;

        static ImGuiLayer* Create();
    };
}