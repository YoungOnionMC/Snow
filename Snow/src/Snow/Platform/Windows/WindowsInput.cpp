#include <spch.h>
#include "Snow/Core/Input.h"

#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
	namespace Core {

		void KeyCallback(KeyCode key, int flags, UINT message) {
			bool pressed = message == WM_KEYDOWN || WM_SYSKEYDOWN;
			bool repeat = (flags >> 30) & 1;

			if (pressed) {
				SNOW_CORE_TRACE("Key Pressed {0}", key);
				Core::Event::KeyPressedEvent event(key, repeat, 0);
				Core::Event::EventSystem::AddEvent(event);
			}
			else {
				Core::Event::KeyReleasedEvent event(key);
				Core::Event::EventSystem::AddEvent(event);
			}
		}

		void MouseButtonCallback(MouseCode button, bool pressed) {
			if (pressed) {
				Core::Event::MouseButtonPressedEvent event(button);
				Core::Event::EventSystem::AddEvent(event);
			}
			else {
				Core::Event::MouseButtonReleasedEvent event(button);
				Core::Event::EventSystem::AddEvent(event);
			}
		}

		void MouseMoveCallback(int xPos, int yPos) {
			Core::Event::MouseMovedEvent event(xPos, yPos);
			Core::Event::EventSystem::AddEvent(event);
		}

		void MouseScrollCallback(double xOffset, double yOffset) {
			Core::Event::MouseScrollEvent event(xOffset, yOffset);
			Core::Event::EventSystem::AddEvent(event);
		}

		bool Input::PlatformInit() {
			return true;
		}
	}
}