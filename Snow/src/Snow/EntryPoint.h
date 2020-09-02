#pragma once

#include "Snow/Core/Log.h"

extern Snow::Core::Application* Snow::Core::CreateApplication();

int main() {
    Snow::Core::Logger::Init();
    auto app = Snow::Core::CreateApplication();
    app->Run();
    delete app;

}