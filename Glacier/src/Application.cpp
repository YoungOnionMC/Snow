#include "Snow/Core/Application.h"
#include "Snow/EntryPoint.h"

#include "EditorLayer.h"

namespace Snow {
    class MainApplication : public Core::Application {
    public:
        MainApplication() {
            PushLayer(new EditorLayer());
        }
    };

    Core::Application* Core::CreateApplication() {
        return new MainApplication();
    }
}
