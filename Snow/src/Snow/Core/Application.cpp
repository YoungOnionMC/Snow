#include <spch.h>

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Core {
        Application::Application() {
            SNOW_CORE_TRACE("Creating Application");


            m_Window = new Window();
        }

        Application::~Application() {
            SNOW_CORE_TRACE("Destroying Application");
        }

        void Application::Run() {
            while(1);
        }
    }
}