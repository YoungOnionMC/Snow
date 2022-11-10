#pragma once

#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
    class VulkanImGuiLayer : public ImGuiLayer {
    public:
        VulkanImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void BeginImGuiFrame() override;
        void EndImGuiFrame() override;

        std::vector<VkDescriptorSet>& GetTextureIDs() { return s_TextureIDs; }
    private:
        std::vector<VkDescriptorSet> s_TextureIDs;
    };
}