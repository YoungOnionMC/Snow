#pragma once
#include "Snow/Render/RenderContext.h"
#include "Snow/Render/RenderCommand.h"

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

            static void BeginScene();
            static void EndScene();
            static void SetViewport(int width, int height);

            static void SwapBuffers();


            static RenderAPI GetRenderAPI() { return s_RenderAPI; }

            static Renderer* Get() { return s_Instance; }
            static Context* GetContext() {return s_Context; }

        private:
            static Renderer* s_Instance;

            static RenderAPI s_RenderAPI;
            static Context* s_Context;

            static RenderCommand* s_RenderCommand;
        };
    }
}