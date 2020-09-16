#pragma once

#include "Snow/Render/RenderCommand.h"

#include <vulkan/vulkan.h>

namespace Snow {
    namespace Render {
        class VulkanRenderCommand : public RenderCommand {
        public:
            VulkanRenderCommand();

            virtual void BeginScene() override;
            virtual void EndScene() override;

            virtual void SetViewport(int width, int height) override;
        private:
            VkCommandBuffer m_DrawCommandBuffer;
        };
    }
}