#include <spch.h>
#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Application.h"


namespace Snow {
    namespace Core {

        Window::Window() {
            SNOW_CORE_INFO("Creating Window...");

            m_WindowResizeListener = new Event::WindowResizeListener();
            m_WindowMoveListener = new Event::WindowMoveListener();
            Event::EventSystem::AddListener(m_WindowResizeListener);
            Event::EventSystem::AddListener(m_WindowMoveListener);

            if(!PlatformInit()) {
                SNOW_CORE_ERROR("Window creation failed");
            }

            SNOW_CORE_INFO("Window Created");
        }

        Window::~Window() {
            SNOW_CORE_INFO("Destroying Window...");
            if(!PlatformShutdown())
                SNOW_CORE_ERROR("Failed to shutdown window");

            SNOW_CORE_INFO("Window destroyed");
        }

        void Window::OnUpdate() {
            PlatformUpdate();
        }
    }
}