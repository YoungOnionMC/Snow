#include <spch.h>
#include "Snow/Core/Input.h"

#include "Snow/Core/Event/EventSystem.h"

#include "Snow/Core/Application.h"

#if defined(SNOW_WINDOW_GLFW)
	#include <GLFW/glfw3.h>
#endif

namespace Snow {
	namespace Core {

#if defined(SNOW_WINDOW_WIN32)
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
#elif defined(SNOW_WINDOW_GLFW)
		void KeyCallback(GLFWwindow* window, int keycode, int scan, int action, int mods) {

			bool pressed = (action == GLFW_PRESS);
			bool repeat = (scan >> 30) & 1;

			int mod = 0;

			Core::Input::SetKeyState((KeyCode)keycode, pressed);
			if (pressed) {
				Core::Event::KeyPressedEvent event((KeyCode)keycode, repeat, mod);
				Core::Event::EventSystem::AddEvent(event);
			}
			else {
				Core::Event::KeyReleasedEvent event((KeyCode)keycode);
				Core::Event::EventSystem::AddEvent(event);
			}
		}


		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

			bool pressed = (action == GLFW_PRESS);

			Core::Input::SetMouseState((MouseCode)button, pressed);
			if (pressed) {
				Core::Event::MouseButtonPressedEvent event((MouseCode)button);
				Core::Event::EventSystem::AddEvent(event);
			}
			else {
				Core::Event::MouseButtonReleasedEvent event((MouseCode)button);
				Core::Event::EventSystem::AddEvent(event);
			}
		}

		void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {
			Core::Input::SetMousePos((int)xPos, (int)yPos);
			Core::Event::MouseMovedEvent event((int)xPos, (int)yPos);
			Core::Event::EventSystem::AddEvent(event);
		}

		void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
			Core::Input::SetMouseScrollOffset((float)xOffset, (float)yOffset);
			Core::Event::MouseScrollEvent event((float)xOffset, (float)xOffset);
			Core::Event::EventSystem::AddEvent(event);
		}
#endif

		bool Input::PlatformInit() {

#if defined(SNOW_WINDOW_WIN32)
			SNOW_CORE_TRACE("Windows input initializing using Win32");
#elif defined(SNOW_WINDOW_GLFW)
			SNOW_CORE_TRACE("Windows input initializing using GLFW");

			auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
			glfwSetKeyCallback(win, KeyCallback);
			glfwSetMouseButtonCallback(win, MouseButtonCallback);
			glfwSetCursorPosCallback(win, MouseMoveCallback);
			glfwSetScrollCallback(win, MouseScrollCallback);
#endif
			return true;
		}
	}
}