#pragma once
#include "Snow/Render/RenderContext.h"
#include "Snow/Render/RenderCommand.h"
#include "Snow/Render/Pipeline.h"

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Event/EventListener.h"
#include "Snow/Core/Window.h"

namespace Snow {
    namespace Render {
        namespace Event {
            class RenderResizeListener : public Core::Event::BaseListener {
            public:
                RenderResizeListener();

                virtual void HandleEvent(Core::Event::BaseEvent* event);
            };
        }

        class Renderer {
        public:
            static void Init();

            static void BeginRenderPass(Ref<RenderPass> renderPass, bool clear = true);
            static void EndRenderPass();

            //static void SwapBuffers();

            static void SetRenderAPI(RenderAPIType api) { s_RenderAPI = api; }
            static RenderAPIType GetRenderAPI() { return s_RenderAPI; }

            static Renderer* Get() { return s_Instance; }
            static Context* GetContext() {return s_Context; }

        private:
            static Renderer* s_Instance;

            static RenderAPIType s_RenderAPI;
            static Context* s_Context;
        };
    }
}