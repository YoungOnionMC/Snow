#pragma once

#include "Snow/Core/Layer.h"

#include <imgui.h>

namespace Snow {
    class ImGuiLayer : public Core::Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        void Begin();
        void End();

    private:



    };
}