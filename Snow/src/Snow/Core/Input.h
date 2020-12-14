#pragma once

#include "Snow/Core/InputCodes.h"

#include "Snow/Core/Event/Event.h"

#include <glm/glm.hpp>

namespace Snow {
    namespace Core {
        class Input {
        public:

            static void Init();

            static bool IsKeyPressed(KeyCode key) { return m_KeyState[static_cast<uint16_t>(key)]; }
            static bool IsMouseButtonPressed(MouseCode button) { return m_MouseState[static_cast<uint8_t>(button)]; }

            static void ClearState();

            static void SetKeyState(KeyCode key, bool pressed) { m_KeyState[static_cast<uint16_t>(key)] = pressed; }
            static void SetMouseState(MouseCode button, bool pressed) { m_MouseState[static_cast<uint8_t>(button)] = pressed; }


            static const glm::vec2& GetMousePos() { return m_MousePosition; }
            static const glm::vec2& GetMouseScrollOffset() { return m_MouseScroll; }

            static void SetMousePos(float xPos, float yPos) { m_MousePosition = { xPos, yPos }; }
            static void SetMouseScrollOffset(float xScroll, float yScroll) { m_MouseScroll = { xScroll, yScroll }; }

            using EventCallbackFn = std::function<void(Event::Event&)>;
            static void SetEventCallback(const EventCallbackFn& callback);

        private:
            static bool PlatformInit();

            // Maybe not needed
            static bool m_KeyState[1024];
            static bool m_MouseState[32];

            static glm::vec2 m_MousePosition;
            static glm::vec2 m_MouseScroll;


        };
    }
}
