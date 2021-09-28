#include <spch.h>
#include "Snow/Core/Input.h"

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Event/KeyEvent.h"
#include "Snow/Core/Event/MouseEvent.h"

#include "Snow/Core/Application.h"

#if defined(SNOW_WINDOW_GLFW)
	#include <GLFW/glfw3.h>
#endif

namespace Snow {
	namespace Core {

		std::function<void(Event::Event&)> InputEventCallback;

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
		static int GLFWToSnowKeyMods(int mods) {
			int result = 0;
			if (mods & GLFW_MOD_SHIFT)
				result |= (int)KeyModifiers::Shift;
			if (mods & GLFW_MOD_ALT)
				result |= (int)KeyModifiers::Alt;
			if (mods & GLFW_MOD_CONTROL)
				result |= (int)KeyModifiers::Control;
			if (mods & GLFW_MOD_CAPS_LOCK)
				result |= (int)KeyModifiers::CapsLock;
			if (mods & GLFW_MOD_SUPER)
				result |= (int)KeyModifiers::Super;
			if (mods & GLFW_MOD_NUM_LOCK)
				result |= (int)KeyModifiers::NumLock;

			return result;
		}

		void KeyCallback(GLFWwindow* window, int keycode, int scan, int action, int mods) {

			
			bool repeat = (scan >> 30) & 1;


			int mod = GLFWToSnowKeyMods(mods);

			switch (action) {
			case GLFW_PRESS: {
				Core::Input::SetKeyState((KeyCode)keycode, true);
				Core::Event::KeyPressedEvent event((KeyCode)keycode, 0, mod);
				InputEventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				Core::Input::SetKeyState((KeyCode)keycode, false);
				Core::Event::KeyReleasedEvent event((KeyCode)keycode);
				InputEventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				Core::Input::SetKeyState((KeyCode)keycode, true);
				Core::Event::KeyPressedEvent event((KeyCode)keycode, repeat, mod);
				InputEventCallback(event);
				break;
			}
			}
		}


		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

			bool pressed = (action == GLFW_PRESS);

			Core::Input::SetMouseState((MouseCode)button, pressed);
			if (pressed) {
				Core::Event::MouseButtonPressedEvent event((MouseCode)button);
				InputEventCallback(event);
			}
			else {
				Core::Event::MouseButtonReleasedEvent event((MouseCode)button);
				InputEventCallback(event);
			}
		}

		void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos) {
			Core::Input::SetMousePos(xPos, yPos);
			Core::Event::MouseMovedEvent event(xPos, yPos);
			InputEventCallback(event);
		}

		void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
			Core::Input::SetMouseScrollOffset(xOffset, yOffset);
			Core::Event::MouseScrolledEvent event(xOffset, xOffset);
			InputEventCallback(event);
		}
#endif

		void Input::SetCursorMode(CursorMode mode) {
#if defined(SNOW_WINDOW_WIN32)
			
#elif defined(SNOW_WINDOW_GLFW)

			auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
#endif
		}

		CursorMode Input::GetCursorMode() {
#if defined(SNOW_WINDOW_WIN32)

#elif defined(SNOW_WINDOW_GLFW)

			auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
			return (CursorMode)(glfwGetInputMode(win, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
#endif
		}

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

		void Input::SetEventCallback(const std::function<void(Event::Event&)>& callback) {
			InputEventCallback = callback;
		}
	}
}