#pragma once

#include "Snow/Render/RenderCommand.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
    namespace Render {
        class VulkanRenderCommand : public RenderCommand {
        public:
            VulkanRenderCommand();

            virtual void BeginScene() override;
            virtual void EndScene() override;

            virtual void DrawIndexed(uint32_t indexCount, PrimitiveType type) override {}

            virtual void SetViewport(int width, int height) override;
        private:
            VkCommandBuffer m_DrawCommandBuffer;
        };
    }
}