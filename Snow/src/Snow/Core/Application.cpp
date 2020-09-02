#include <iostream>

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Core {
        Application::Application() {
            std::cout << "Creating Application\n";
        }

        Application::~Application() {
            std::cout << "Destroying Application\n";
        }
    }
}