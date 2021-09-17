#include <spch.h>
#include "Snow/Core/Window.h"

#include "Snow/Core/Base.h"

#include "Snow/Core/Application.h"


namespace Snow {
    namespace Core {

        Window::Window() {
            SNOW_CORE_INFO("Creating Window...");

            

            
        }

        Window::~Window() {
            SNOW_CORE_INFO("Destroying Window...");
            if(!PlatformShutdown())
                SNOW_CORE_ERROR("Failed to shutdown window");

            SNOW_CORE_INFO("Window destroyed");
        }

        void Window::ProcessEvents() {
            ProcessEventsInternal();
        }

        void Window::SwapBuffers() {
            SwapBuffersInternal();
        }
    }
}