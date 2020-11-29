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

            static void SetMousePos(int xPos, int yPos) { m_MousePosition = { xPos, yPos }; }
            static void SetMouseScrollOffset(float xScroll, float yScroll) { m_MouseScroll = { xScroll, yScroll }; }

        private:
            static bool PlatformInit();

            // Maybe not needed
            static bool m_KeyState[1024];
            static bool m_MouseState[32];

            static glm::vec2 m_MousePosition;
            static glm::vec2 m_MouseScroll;


        };




        namespace Event {

            struct KeyPressedEvent : public Event<KeyPressedEvent> {
            public:
                KeyPressedEvent(KeyCode keycode, int repeat, int modifiers) :
                    m_KeyCode(keycode), m_Repeat(repeat), m_Mods(modifiers) {}

                inline KeyCode GetKeyCode() const { return m_KeyCode; }
                inline int GetRepeat() const { return m_Repeat; }
                inline int GetModifiers() const { return m_Mods; }

            private:
                KeyCode m_KeyCode;
                int m_Repeat, m_Mods;
            };

            struct KeyReleasedEvent : public Event<KeyReleasedEvent> {
            public:
                KeyReleasedEvent(KeyCode keycode) : 
                    m_KeyCode(keycode) {}

                inline KeyCode GetKeyCode() const { return m_KeyCode; }
            private:
                KeyCode m_KeyCode;
            };

            struct KeyTypedEvent : public Event<KeyTypedEvent> {
                
            };

            struct MouseButtonPressedEvent : public Event<MouseButtonPressedEvent> {
            public:
                MouseButtonPressedEvent(MouseCode button) :
                    m_MouseCode(button) {}

                inline MouseCode GetMouseCode() const { return m_MouseCode; }
            private:
                MouseCode m_MouseCode;
            };

            struct MouseButtonReleasedEvent : public Event<MouseButtonReleasedEvent> {
            public:
                MouseButtonReleasedEvent(MouseCode button) :
                    m_MouseCode(button) {}

                inline MouseCode GetMouseCode() const { return m_MouseCode; }
            private:
                MouseCode m_MouseCode;
            };

            struct MouseMovedEvent : public Event<MouseMovedEvent> {
            public:
				MouseMovedEvent(int x, int y) :
					m_MouseX(x), m_MouseY(y) {}

                inline int GetX() const { return m_MouseX; }
				inline int GetY() const { return m_MouseY; }
            private:
                int m_MouseX, m_MouseY;
            };

            struct MouseScrollEvent : public Event<MouseScrollEvent> {
            public:
				MouseScrollEvent(float xOffset, float yOffset) :
					m_XOffset(xOffset), m_YOffset(yOffset) {}

				inline float GetXOffset() const { return m_XOffset; }
				inline float GetYOffset() const { return m_YOffset; }
            private:
                float m_XOffset, m_YOffset;
            };
        }
    }
}
