#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Core/Log.h"

#include "Snow/Core/Input.h"


namespace Snow {
    namespace Core {
        class Window : public RefCounted {
        public:

            Window();
            ~Window();

            void OnUpdate();

            uint32_t GetWidth();
            uint32_t GetHeight();

            void* GetWindowHandle();

            using EventCallbackFn = std::function<void(Event::Event&)>;
            void SetEventCallback(const EventCallbackFn& callback);
        private:


            bool PlatformInit();
            bool PlatformShutdown();

            void PlatformUpdate();
        };

        
    }
}