#include <spch.h>

#include "Snow/Core/Application.h"
#include "Snow/Render/Renderer.h"

#include "Snow/Render/Renderer2D.h"

namespace Snow {
    namespace Core {


        Application* Application::s_Instance = nullptr;

       

        Application::Application() {
            SNOW_CORE_INFO("Creating Application...");
            s_Instance = this;
            Event::EventSystem::Init();

            m_AppRenderListener = new Event::ApplicationRenderListener();
            m_AppCloseListener = new Event::ApplicationCloseListener();
            Event::EventSystem::AddListener(m_AppRenderListener);
            Event::EventSystem::AddListener(m_AppCloseListener);


            m_Window = new Window();
            Input::Init();


            //m_Window->SetEventCallback(SNOW_BIND_EVENT_FN(Application::OnEvent));

            Render::Renderer::Init();
        }

        Application::~Application() {
            delete m_Window;
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::Run() {
            while(m_Running) {
                OnUpdate();

                Render::Renderer::BeginScene();
                Render::Renderer2D::DrawQuad({0.0f, 0.0f},{1.0f , 1.0f});
                Render::Renderer::EndScene();
            }
        }

        void Application::OnUpdate() {
            m_Window->OnUpdate();

            Event::EventSystem::Get().ProcessEvents();
        }
    }
}