#include <spch.h>

#include "Snow/Core/Application.h"
#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Core {


        Application* Application::s_Instance = nullptr;

        Application::Application() {
            SNOW_CORE_TRACE("Creating Application");
            s_Instance = this;
            Event::EventSystem::Init();


            m_Window = new Window();


            //m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));

            Render::Renderer::Init();
        }

        Application::~Application() {
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::Run() {
            while(m_Running) {
                OnUpdate();
            }
        }

        void Application::OnUpdate() {
            m_Window->OnUpdate();

            Event::EventSystem::Get().ProcessEvents();
        }
    }
}