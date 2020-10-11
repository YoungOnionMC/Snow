#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Core/Window.h"
#include "Snow/Core/Input.h"

#include "Snow/Core/Base.h"
#include "Snow/Core/Event/EventSystem.h"

#include "Snow/Core/Layer.h"
#include "Snow/ImGui/ImGuiLayer.h"

namespace Snow {
    namespace Core {

        
        namespace Event {
        class ApplicationRenderListener;
        class ApplicationCloseListener;
        };

        class Application {
        public:
            /* @brief
            */
            Application();
            ~Application();

            void Run();


            static Application& Get() { return *s_Instance; }
            Ref<Window> GetWindow() { return Get().m_Window; }

            void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
            void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

            virtual void OnUpdate();

            void OnImGuiRender();

            inline void SetRunning(bool running) { m_Running = running; }
        private:

            static Application* s_Instance;

            Ref<Window> m_Window;

            LayerStack m_LayerStack;
            ImGuiLayer* m_ImGuiLayer;

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
