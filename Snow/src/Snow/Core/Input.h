#pragma once

#include "Snow/Core/InputCodes.h"

#include "Snow/Core/Event/Event.h"

#include <glm/glm.hpp>

#include <map>

namespace Snow {
    namespace Core {

        enum class CursorMode {
            Normal = 0,
            Hidden = 1,
            Locked = 2
        };

        struct Controller {
            int ID;
            std::string Name;
            std::map<int, bool> ButtonStates;
            std::map<int, float> AxisStates;
            std::map<int, uint8_t> HatStates;
        };

        class Input {
        public:

            static void Init();

            static bool IsKeyPressed(KeyCode key) { return m_KeyState[static_cast<uint16_t>(key)]; }
            static bool IsMouseButtonPressed(MouseCode button) { return m_MouseState[static_cast<uint8_t>(button)]; }

            static void Update();
            static void ClearState();

            static void SetKeyState(KeyCode key, bool pressed) { m_KeyState[static_cast<uint16_t>(key)] = pressed; }
            static void SetMouseState(MouseCode button, bool pressed) { m_MouseState[static_cast<uint8_t>(button)] = pressed; }

            static void SetCursorMode(CursorMode mode);
            static CursorMode GetCursorMode();

            static const glm::vec2& GetMousePos() { return m_MousePosition; }
            static const glm::vec2& GetMouseScrollOffset() { return m_MouseScroll; }

            static void SetMousePos(float xPos, float yPos) { m_MousePosition = { xPos, yPos }; }
            static void SetMouseScrollOffset(float xScroll, float yScroll) { m_MouseScroll = { xScroll, yScroll }; }

            static bool IsControllerPresent(int id);
            static std::vector<int> GetConnectedControllers();
            static const Controller* GetController(int id);
            static bool IsControllerButtonPressed(int controllerID, int button);
            static float GetControllerAxis(int controllerID, int axis);
            static uint8_t GetControllerHat(int controllerID, int hat);

            static const std::map<int, Controller>& GetControllers() { return s_Controllers; }

            using EventCallbackFn = std::function<void(Event::Event&)>;
            static void SetEventCallback(const EventCallbackFn& callback);

        private:
            static bool PlatformInit();

            // Maybe not needed
            static bool m_KeyState[1024];
            static bool m_MouseState[32];

            static glm::vec2 m_MousePosition;
            static glm::vec2 m_MouseScroll;

            static std::map<int, Controller> s_Controllers;
        };
    }
}
