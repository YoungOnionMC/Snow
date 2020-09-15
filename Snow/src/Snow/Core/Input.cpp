#include <spch.h>

#include "Snow/Core/Input.h"

namespace Snow {
    namespace Core {

        bool Input::m_KeyState[1024] = {};
        bool Input::m_MouseState[32] = {};

        Math::Vector2i Input::m_MousePosition = {};
        Math::Vector2f Input::m_MouseScroll = {};


        void Input::Init() {
            SNOW_CORE_TRACE("Input Initialzed");
            if(!PlatformInit())
                SNOW_CORE_ERROR("Platform Init failed");
        }
        

    }
}