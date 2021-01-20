#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Core/Window.h"
#include "Snow/Core/Input.h"

#include "Snow/Core/Base.h"
#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Event/ApplicationEvent.h"

#include "Snow/Core/Layer.h"
#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Core/Timestep.h"


namespace Snow {
    namespace Core {

        class Application {
        public:
            Application();
            ~Application();

            void Run();

            void OnEvent(Event::Event& event);


            static Application& Get() { return *s_Instance; }
            Ref<Window> GetWindow() { return Get().m_Window; }

            void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
            void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

            virtual void OnUpdate(Timestep ts);

            void OnImGuiRender();

            void Close() { m_Running = false; }
        private:

            bool OnApplicationUpdate(Event::AppUpdateEvent& e);
            bool OnApplicationResize(Event::WindowResizeEvent& e);
            bool OnApplicationClose(Event::WindowCloseEvent& e);

            static Application* s_Instance;

            Ref<Window> m_Window;

            LayerStack m_LayerStack;
            ImGuiLayer* m_ImGuiLayer;

            bool m_Running = true;
            float m_LastFrameTime = 0.0f;
        };

        Application* CreateApplication();
    }
}
