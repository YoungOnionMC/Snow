#pragma once

#include "Snow/Render/RenderCommand.h"

#include "Snow/Platform/Vulkan/VulkanCommon.h"

namespace Snow {
    namespace Render {
        class VulkanRenderCommand : public RenderAPI {
        public:
            VulkanRenderCommand();

            virtual void DrawIndexed(uint32_t indexCount, PrimitiveType type) override {}

            virtual void SetViewport(uint32_t width, uint32_t height) override;

            virtual void BeginCommandBuffer() override;
            virtual void EndCommandBuffer() override;

            virtual void SwapBuffers() override;
        private:
            VkCommandBuffer m_DrawCommandBuffer;
        };
    }
}