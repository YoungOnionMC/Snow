#pragma once

#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {

        namespace Event {
            struct AppUpdateEvent : public Event {
            public:
                AppUpdateEvent() = default;
            };

            class AppRenderEvent : public Event {
            public:
                AppRenderEvent() = default;
            };


            class ApplicationListener : public Listener {
            public:
                virtual void HandleEvent(Event& event) override {
                    SNOW_CORE_TRACE("AppRenderEvent");
                }
            };
        }



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
        };

        Application* CreateApplication();
    }
}
