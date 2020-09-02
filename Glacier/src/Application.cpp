#include "Snow/Core/Application.h"
#include "Snow/EntryPoint.h"

class MainApplication : public Snow::Core::Application {
public:
    MainApplication() {

    }
};



Snow::Core::Application* Snow::Core::CreateApplication() {
    return new MainApplication();
}
