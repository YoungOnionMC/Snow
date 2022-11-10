#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Core/Log.h"

#include "Snow/Render/RenderContext.h"
#include "Snow/Render/SwapChain.h"

#include "Snow/Core/Event/Event.h"

namespace Snow {
    namespace Core {
        class Window : public RefCounted {
        public:

            Window();
            ~Window();

            bool PlatformInit();

            void ProcessEvents();
            void SwapBuffers();

            uint32_t GetWidth();
            uint32_t GetHeight();

            void* GetWindowHandle();
            float GetSystemTime();

            virtual Ref<Render::SwapChain> GetSwapChain();
            virtual Ref<Render::Context> GetRenderContext();

            using EventCallbackFn = std::function<void(Event::Event&)>;
            void SetEventCallback(const EventCallbackFn& callback);
        private:


            
            bool PlatformShutdown();

            void ProcessEventsInternal();
            void SwapBuffersInternal();

            Ref<Render::Context> m_RendererContext;
            Ref<Render::SwapChain> m_SwapChain;
        };

        
    }
}