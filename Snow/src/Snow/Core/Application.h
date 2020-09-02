#pragma once

#include "Snow/Core/Window.h"

namespace Snow {
    namespace Core {
        class Application {
        public:
            Application();
            ~Application();

            void Run();


            static Application& Get() { return *s_Instance; }
            Window* GetWindow() { return Get().m_Window; }
        private:
            static Application* s_Instance;

            Window* m_Window;
        };

        Application* CreateApplication();
    }
}
