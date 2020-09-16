#include <spch.h>
#include "Snow/Core/Input.h"

#include "Snow/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Snow {

    void KeyCallback(GLFWwindow* window, int keycode, int scan, int action, int mods) {

        bool pressed = (action == GLFW_PRESS);
        bool repeat = (scan >> 30) & 1;

        int mod = 0;

        Core::Input::SetKeyState((KeyCode)keycode, pressed);
        if(pressed) {
            Core::Event::KeyPressedEvent event((KeyCode)keycode, repeat, mod);
            Core::Event::EventSystem::AddEvent(event);  
        } else {
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
        } else {
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

    bool Core::Input::PlatformInit() {
        SNOW_CORE_INFO("Linux Input Initializing using GLFW");

        auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());
        glfwSetKeyCallback(win, KeyCallback);

        glfwSetMouseButtonCallback(win, MouseButtonCallback);
        glfwSetCursorPosCallback(win, MouseMoveCallback);
        glfwSetScrollCallback(win, MouseScrollCallback);

        return true;
    }
}