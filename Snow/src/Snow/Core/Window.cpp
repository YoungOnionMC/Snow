#include <spch.h>
#include "Snow/Core/Window.h"


namespace Snow {
    namespace Core {
        Window::Window() {
            SNOW_CORE_TRACE("Creating Window");
            if(!PlatformInit());
                //"Window creation failed";
        }

        Window::~Window() {
            SNOW_CORE_TRACE("Destroying Window");
        }
    }
}