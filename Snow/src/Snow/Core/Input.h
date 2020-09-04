#pragma once

#include "Snow/Core/InputCodes.h"

namespace Snow {
    namespace Core {
        class Input {
        public:
            static void Init();

            static bool IsKeyPressed(KeyCode key) { return m_KeyState[static_cast<uint16_t>(key)]; }
            static bool IsMouseButtonPressed(MouseCode button) { return m_MouseState[static_cast<uint8_t>(button)]; }

            static void ClearState();

        private:
            static bool m_KeyState[1024];
            static bool m_MouseState[32];

            


        };
    }
}