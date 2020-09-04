#include <spch.h>
#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Application.h"


namespace Snow {
    namespace Core {

        Window::Window() {
            SNOW_CORE_TRACE("Creating Window");
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