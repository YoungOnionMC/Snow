#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Core/Event/EventSystem.h"

#include "Snow/Core/Log.h"

#include "Snow/Core/Input.h"

namespace Snow {
    namespace Core {

        namespace Event {
            struct WindowCloseEvent : public Event<WindowCloseEvent> {
            public:
                WindowCloseEvent() {}
            };

            struct WindowMinimizedEvent : public Event<WindowMinimizedEvent> {
            public:
                WindowMinimizedEvent() {}
            };

            struct WindowResizeEvent : public Event<WindowResizeEvent> {
            public:
                WindowResizeEvent(uint32_t width, uint32_t height) :
                    m_Width(width), m_Height(height) {}

                uint32_t GetWidth() { return m_Width; }
                uint32_t GetHeight() { return m_Height; }

            private:
                uint32_t m_Width, m_Height;
            };

            struct WindowMovedEvent : public Event<WindowMovedEvent> {
            public:
                WindowMovedEvent(int32_t xPos, int32_t yPos) :
                    m_XPos(xPos), m_YPos(yPos) {}
                    
                int32_t GetXPos() { return m_XPos; }
                int32_t GetYPos() { return m_YPos; }
            private:
                int32_t m_XPos, m_YPos;      
            };


            struct WindowFullscreenEvent : public Event<WindowFullscreenEvent> {
            public:
                WindowFullscreenEvent(int32_t width, int32_t height) : 
                    m_Width(width), m_Height(height) {}
                
                int32_t GetWidth() { return m_Width; }
                int32_t GetHeight() { return m_Height; }
            private:
                int32_t m_Width, m_Height;
            };


            class WindowResizeListener : public BaseListener {
            public:
                WindowResizeListener() {
                    SetEventType(WindowResizeEvent::ID);
                }

                virtual void HandleEvent(BaseEvent* event) override {
                   WindowResizeEvent* e = (WindowResizeEvent*)event;
                   SNOW_CORE_TRACE("Window width {0}, height {1}", e->GetWidth(), e->GetHeight());
                }
            };

            class WindowMoveListener : public BaseListener {
                public:
                WindowMoveListener() {
                    SetEventType(WindowMovedEvent::ID);
                }

                virtual void HandleEvent(BaseEvent* event) override {
                   WindowMovedEvent* e = (WindowMovedEvent*)event;
                   //SNOW_CORE_TRACE("Window X {0}, Y {1}", e->GetXPos(), e->GetYPos());
                }
            };
        }

        class Window : public RefCounted {
        public:

            Window();
            ~Window();

            void OnUpdate();

            uint32_t GetWidth();
            uint32_t GetHeight();

            void* GetWindowHandle();
        private:


            bool PlatformInit();
            bool PlatformShutdown();

            void PlatformUpdate();

            Event::WindowResizeListener* m_WindowResizeListener;
            Event::WindowMoveListener* m_WindowMoveListener;
        };

        
    }
}