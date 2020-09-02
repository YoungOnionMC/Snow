#include <spch.h>

#include "Snow/Core/Application.h"
#include "Snow/Render/Renderer.h"

namespace Snow {
    namespace Core {
        Application* Application::s_Instance = nullptr;

        Application::Application() {
            SNOW_CORE_TRACE("Creating Application");
            s_Instance = this;

            m_Window = new Window();

            Render::Renderer::Init();
        }

        Application::~Application() {
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::Run() {
            while(1);
        }
    }
}