#include <spch.h>
#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Application.h"


namespace Snow {
    namespace Core {

        Window::Window() {
            SNOW_CORE_TRACE("Creating Window");

            m_WindowResizeListener = new Event::WindowResizeListener();
            m_WindowMoveListener = new Event::WindowMoveListener();
            Event::EventSystem::AddListener(m_WindowResizeListener);
            Event::EventSystem::AddListener(m_WindowMoveListener);

            if(!PlatformInit()) {
                SNOW_CORE_ERROR("Window creation failed");
            }

            
        }

        Window::~Window() {
            SNOW_CORE_TRACE("Destroying Window");
        }

        void Window::OnUpdate() {
            PlatformUpdate();
        }
    }
}