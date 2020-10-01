#pragma once

#include "Snow/ImGui/ImGuiLayer.h"

namespace Snow {
    class VulkanImGuiLayer : public ImGuiLayer {
    public:
        VulkanImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void BeginImGuiFrame() override;
        void EndImGuiFrame() override;
    };
}