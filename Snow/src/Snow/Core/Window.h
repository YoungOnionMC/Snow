#pragma once

namespace Snow {
    namespace Core {
        class Window {
        public:
            Window();
            ~Window();

            void* GetWindowHandle();
        private:
            bool PlatformInit();
            bool PlatformShutdown();


        };
    }
}