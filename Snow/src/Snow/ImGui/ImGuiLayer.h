#pragma once

#include "Snow/Core/Layer.h"

#include <imgui.h>
#if defined(SNOW_WINDOW_GLFW)
    #include <glfw/glfw3.h>

    #if defined(SNOW_PLATFORM_WINDOWS)
        #define GLFW_EXPOSE_NATIVE_WIN32
    #endif

    #include <glfw/glfw3native.h>

    #include <examples/imgui_impl_glfw.h>
#elif defined(SNOW_WINDOW_WIN32)
    #include <examples/imgui_impl_dx11.h>
#endif

namespace Snow {
    class ImGuiLayer : public Core::Layer {
    public:
        virtual void BeginImGuiFrame() = 0;
        virtual void EndImGuiFrame() = 0;

        static ImGuiLayer* Create();
    };
}