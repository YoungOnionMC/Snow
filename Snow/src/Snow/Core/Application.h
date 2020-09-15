#pragma once

#include "Snow/Core/Window.h"
#include "Snow/Core/Input.h"

#include "Snow/Core/Base.h"
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {

        
        namespace Event {
        class ApplicationRenderListener;
        class ApplicationCloseListener;
        };

        class Application {
        public:
            Application();
            ~Application();

            void Run();


            static Application& Get() { return *s_Instance; }
            Window* GetWindow() { return Get().m_Window; }

            virtual void OnUpdate();

            inline void SetRunning(bool running) { m_Running = running; }
        private:

            static Application* s_Instance;

            Window* m_Window;

            bool m_Running = true;

            Event::ApplicationRenderListener* m_AppRenderListener;
            Event::ApplicationCloseListener* m_AppCloseListener;
        };

        Application* CreateApplication();


        namespace Event {
            struct AppUpdateEvent : public Event<AppUpdateEvent> {
            public:
                AppUpdateEvent() = default;
            };

            class AppRenderEvent : public Event<AppRenderEvent> {
            public:
                AppRenderEvent() = default;
            };


            class ApplicationRenderListener : public BaseListener {
            public:
                ApplicationRenderListener() {
                    SetEventType(AppRenderEvent::ID);
                }

                virtual void HandleEvent(BaseEvent* event) override {
                    SNOW_CORE_TRACE("AppRenderEvent");
                }
            };

            class ApplicationCloseListener : public BaseListener {
            public:
                ApplicationCloseListener() {
                    SetEventType(WindowCloseEvent::ID);
                }

                virtual void HandleEvent(BaseEvent* event) override {
                    Application::Get().SetRunning(false);
                }
            };
        }
    }
}
