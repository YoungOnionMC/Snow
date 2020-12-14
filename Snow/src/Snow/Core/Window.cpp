#include <spch.h>
#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Application.h"


namespace Snow {
    namespace Core {

        Window::Window() {
            SNOW_CORE_INFO("Creating Window...");

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