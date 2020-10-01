#pragma once

#include "Snow/ImGui/ImGuiLayer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>

namespace Snow {
    class OpenGLImGuiLayer : public ImGuiLayer {
    public:
        OpenGLImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void BeginImGuiFrame() override;
        void EndImGuiFrame() override;
    };
}