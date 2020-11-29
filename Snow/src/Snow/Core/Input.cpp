#include <spch.h>

#include "Snow/Core/Input.h"

namespace Snow {
    namespace Core {

        bool Input::m_KeyState[1024] = {};
        bool Input::m_MouseState[32] = {};

        glm::vec2 Input::m_MousePosition = {};
        glm::vec2 Input::m_MouseScroll = {};


        void Input::Init() {
            SNOW_CORE_INFO("Input Initialzed");
            if(!PlatformInit())
                SNOW_CORE_ERROR("Platform Init failed");
        }
        

    }
}