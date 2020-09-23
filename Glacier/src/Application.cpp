#include "Snow/Core/Application.h"
#include "Snow/EntryPoint.h"

#include "EditorLayer.h"

class MainApplication : public Snow::Core::Application {
public:
    MainApplication() {
        PushLayer(new EditorLayer());
    }
};

Snow::Core::Application* Snow::Core::CreateApplication() {
    return new MainApplication();
}
