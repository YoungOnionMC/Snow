#pragma once

extern Snow::Core::Application* Snow::Core::CreateApplication();

int main() {

    auto app = Snow::Core::CreateApplication();
    delete app;

}