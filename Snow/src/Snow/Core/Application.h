#pragma once

#include "Snow/Core/Window.h"

namespace Snow {
    namespace Core {
        class Application {
        public:
            Application();
            ~Application();

            void Run();
        private:
            Window* m_Window;
        };

        Application* CreateApplication();
    }
}
