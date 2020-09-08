#pragma once

#include <functional>

#include "Snow/Core/Event/EventSystem.h"

#include "Snow/Core/Log.h"

namespace Snow {
    namespace Core {

        namespace Event {
            struct WindowCloseEvent : public Event {
            public:
                WindowCloseEvent() {}
            };

            struct WindowMinimizedEvent : public Event {
            public:
                WindowMinimizedEvent() {}
            };

            struct WindowResizeEvent : public Event {
            public:
                WindowResizeEvent(uint32_t width, uint32_t height) :
                    m_Width(width), m_Height(height) {}

                uint32_t GetWidth() { return m_Width; }
                uint32_t GetHeight() { return m_Height; }

            private:
                uint32_t m_Width, m_Height;

                
                
            };


            class WindowResizeListener : public Listener {
            public:
                WindowResizeListener() = default;

                virtual void HandleEvent(Event* event) override {
                    WindowResizeEvent* e = static_cast<WindowResizeEvent*>(event);
                    SNOW_CORE_TRACE("Window Width {0}", e->GetWidth()); 
                    SNOW_CORE_TRACE("sizeof event {0}", WindowResizeEvent::ID);
                    SNOW_CORE_TRACE("sizeof event {0}", WindowMinimizedEvent::ID);
                }
            };
        }

        class Window {
        public:

            Window();
            ~Window();

            void OnUpdate();

            void* GetWindowHandle();
            Event::WindowResizeListener* m_WindowResizeListener;
        private:


            bool PlatformInit();
            bool PlatformShutdown();

            void PlatformUpdate();
        };

        
    }
}