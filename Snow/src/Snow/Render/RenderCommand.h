#pragma once

#include "Snow/Core/Ref.h"
#include "Snow/Render/API/Buffer.h"

#include "Snow/Render/Pipeline.h"

#include "Snow/Math/Vector/Vector4.h"

namespace Snow {
    namespace Render {
        class RenderAPI {
        public:

            virtual void Init() = 0;
            virtual void SetViewport(uint32_t width, uint32_t height) = 0;
            virtual void SetClearColor(const Math::Vector4f& color) = 0;
            virtual void Clear() = 0;

            virtual void DrawIndexed(uint32_t count, PrimitiveType type) = 0;

            virtual void SwapBuffers() = 0;

            static Core::Scope<RenderAPI> Create();

        };

        class RenderCommand {
        public:
            static void Init() {
                s_RenderAPI = RenderAPI::Create();
                s_RenderAPI->Init();
            }

            static void SetClearColor(const Math::Vector4f& color) {
                s_RenderAPI->SetClearColor(color);
            }

            static void Clear() {
                s_RenderAPI->Clear();
            }

            static void DrawIndexed(uint32_t count, PrimitiveType type) {
                s_RenderAPI->DrawIndexed(count, type);
            }

            static void SetViewport(int width, int height) {
                s_RenderAPI->SetViewport(width, height);
            }

            static void SwapBuffers() {
                s_RenderAPI->SwapBuffers();
            }
        private:
            static Core::Scope<RenderAPI> s_RenderAPI;
        };
    }
}