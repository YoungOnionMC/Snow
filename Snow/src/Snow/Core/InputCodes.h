#pragma once
#include <ostream>

namespace Snow {
    typedef enum class KeyCode : uint16_t {

        Space           = 32,
        Apostrophe      = 39,
        Comma           = 44,
        Minus           = 45,
        Period          = 46,
        Slash           = 47,

        D0              = 48,
        D1              = 49,
        D2              = 50,
        D3              = 51,
        D4              = 52,
        D5              = 53,
        D6              = 54,
        D7              = 55,
        D8              = 56,
        D9              = 57,

        Semicolon       = 59,
        Equal           = 61,

        A               = 65,
        B               = 66,
        C               = 67,
        D               = 68,
        E               = 69,
        F               = 70,
        G               = 71,
        H               = 72,
        I               = 73,
        J               = 74,
        K               = 75,
        L               = 76,
        M               = 77,
        N               = 78,
        O               = 79,
        P               = 80,
        Q               = 81,
        R               = 82,
        S               = 83,
        T               = 84,
        U               = 85,
        V               = 86,
        W               = 87,
        X               = 88,
        Y               = 89,
        Z               = 90,

        LeftBracket     = 91,
        Backslash       = 92,
        RightBracket    = 93,
        GraveAccent     = 96,

        World1          = 161,
        World2          = 162,

        Escape          = 256,
        Enter           = 257,
        Tab             = 258,
        Backspace       = 259,
        Insert          = 260,
        Delete          = 261,
        Right           = 262,
        Left            = 263,
        Down            = 264,
        Up              = 265,
        PageUp          = 266,
        PageDown        = 267,
        Home            = 268,
        End             = 269,
        CapsLock        = 280,
        ScrollLock      = 281,
        NumLock         = 282,
        PrintScreen     = 283,
        Pause           = 284,
        F1              = 290,
        F2              = 291,
        F3              = 292,
        F4              = 293,
        F5              = 294,
        F6              = 295,
        F7              = 296,
        F8              = 297,
        F9              = 298,
        F10             = 299,
        F11             = 300,
        F12             = 301,
        F13             = 302,
        F14             = 303,
        F15             = 304,
        F16             = 305,
        F17             = 306,
        F18             = 307,
        F19             = 308,
        F20             = 309,
        F21             = 310,
        F22             = 311,
        F23             = 312,
        F24             = 313,
        F25             = 314,

        KP0             = 320,
        KP1             = 321,
        KP2             = 322,
        KP3             = 323,
        KP4             = 324,
        KP5             = 325,
        KP6             = 326,
        KP7             = 327,
        KP8             = 328,
        KP9             = 329,
        KPDecimal       = 330,
        KPDivide        = 331,
        KPMultiply      = 332,
        KPSubtract      = 333,
        KPAdd           = 334,
        KPEnter         = 335,
        KPEqual         = 336,

        LeftShift       = 340,
        LeftControl     = 341,
        LeftAlt         = 342,
        LeftSuper       = 343,
        RightShift      = 344,
        RightControl    = 345,
        RightAlt        = 346,
        RightSuper      = 347,
        Menu            = 348

    } Key;

    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) {
        return os << static_cast<uint32_t>(keyCode);
    }

    typedef enum class MouseCode : uint16_t {
        Button0         = 0,
        Button1         = 1,
        Button2         = 2,
        Button3         = 3,
        Button4         = 4,
        Button5         = 5,
        Button6         = 6,
        Button7         = 7,

        ButtonLast      = Button7,
        ButtonLeft      = Button0,
        ButtonRight     = Button1,
        ButtonMiddle    = Button2
    } Mouse;

    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
        return os << static_cast<uint16_t>(mouseCode);
    }
}

#define SNOW_KEY_SPACE          ::Snow::KeyCode::Space
#define SNOW_KEY_APOSTROPHE     ::Snow::KeyCode::Apostrophe
#define SNOW_KEY_COMMA          ::Snow::KeyCode::Comma
#define SNOW_KEY_MINUS          ::Snow::KeyCode::Minus
#define SNOW_KEY_PERIOD         ::Snow::KeyCode::Period
#define SNOW_KEY_SLASH          ::Snow::KeyCode::Slash
		
#define SNOW_KEY_0				::Snow::KeyCode::D0
#define SNOW_KEY_1				::Snow::KeyCode::D1
#define SNOW_KEY_2				::Snow::KeyCode::D2
#define SNOW_KEY_3				::Snow::KeyCode::D3
#define SNOW_KEY_4				::Snow::KeyCode::D4
#define SNOW_KEY_5				::Snow::KeyCode::D5
#define SNOW_KEY_6				::Snow::KeyCode::D6
#define SNOW_KEY_7				::Snow::KeyCode::D7
#define SNOW_KEY_8				::Snow::KeyCode::D8
#define SNOW_KEY_9				::Snow::KeyCode::D9

#define SNOW_KEY_SEMICOLON      ::Snow::KeyCode::Semicolon
#define SNOW_KEY_EQUAL          ::Snow::KeyCode::Equal
		
#define SNOW_KEY_A				::Snow::KeyCode::A
#define SNOW_KEY_B				::Snow::KeyCode::B
#define SNOW_KEY_C				::Snow::KeyCode::C
#define SNOW_KEY_D				::Snow::KeyCode::D
#define SNOW_KEY_E				::Snow::KeyCode::E
#define SNOW_KEY_F				::Snow::KeyCode::F
#define SNOW_KEY_G				::Snow::KeyCode::G
#define SNOW_KEY_H				::Snow::KeyCode::H
#define SNOW_KEY_I				::Snow::KeyCode::I
#define SNOW_KEY_J				::Snow::KeyCode::J
#define SNOW_KEY_K				::Snow::KeyCode::K
#define SNOW_KEY_L				::Snow::KeyCode::L
#define SNOW_KEY_M				::Snow::KeyCode::M
#define SNOW_KEY_N				::Snow::KeyCode::N
#define SNOW_KEY_O				::Snow::KeyCode::O
#define SNOW_KEY_P				::Snow::KeyCode::P
#define SNOW_KEY_Q				::Snow::KeyCode::Q
#define SNOW_KEY_R				::Snow::KeyCode::R
#define SNOW_KEY_S				::Snow::KeyCode::S
#define SNOW_KEY_T				::Snow::KeyCode::T
#define SNOW_KEY_U				::Snow::KeyCode::U
#define SNOW_KEY_V				::Snow::KeyCode::V
#define SNOW_KEY_W				::Snow::KeyCode::W
#define SNOW_KEY_X				::Snow::KeyCode::X
#define SNOW_KEY_Y				::Snow::KeyCode::Y
#define SNOW_KEY_Z				::Snow::KeyCode::Z

#define SNOW_KEY_LEFT_BRACKET   ::Snow::KeyCode::LeftBracket
#define SNOW_KEY_BACKSLASH      ::Snow::KeyCode::Backslash
#define SNOW_KEY_RIGHT_BRACKET  ::Snow::KeyCode::RightBracket
#define SNOW_KEY_GRAVE_ACCENT   ::Snow::KeyCode::GraveAccent
#define SNOW_KEY_WORLD1         ::Snow::KeyCode::World1
#define SNOW_KEY_WORLD2         ::Snow::KeyCode::World2

#define SNOW_KEY_ESCAPE			::Snow::KeyCode::Escape
#define SNOW_KEY_ENTER          ::Snow::KeyCode::Enter
#define SNOW_KEY_TAB            ::Snow::KeyCode::Tab
#define SNOW_KEY_BACKSPACE      ::Snow::KeyCode::Backspace
#define SNOW_KEY_INSERT         ::Snow::KeyCode::Insert
#define SNOW_KEY_DELETE         ::Snow::KeyCode::Delete
#define SNOW_KEY_RIGHT          ::Snow::KeyCode::Right
#define SNOW_KEY_LEFT           ::Snow::KeyCode::Left
#define SNOW_KEY_DOWN           ::Snow::KeyCode::Down
#define SNOW_KEY_UP             ::Snow::KeyCode::Up
#define SNOW_KEY_PAGE_UP        ::Snow::KeyCode::PageUp
#define SNOW_KEY_PAGE_DOWN      ::Snow::KeyCode::PageDown
#define SNOW_KEY_HOME           ::Snow::KeyCode::Home
#define SNOW_KEY_END            ::Snow::KeyCode::End
#define SNOW_KEY_CAPS_LOCK      ::Snow::KeyCode::CapsLock
#define SNOW_KEY_SCROLL_LOCK    ::Snow::KeyCode::ScrollLock
#define SNOW_KEY_NUM_LOCK       ::Snow::KeyCode::NumLock
#define SNOW_KEY_PRINT_SCREEN   ::Snow::KeyCode::PrintScreen
#define SNOW_KEY_PAUSE          ::Snow::KeyCode::Pause
#define SNOW_KEY_F1             ::Snow::KeyCode::F1
#define SNOW_KEY_F2             ::Snow::KeyCode::F2
#define SNOW_KEY_F3             ::Snow::KeyCode::F3
#define SNOW_KEY_F4             ::Snow::KeyCode::F4
#define SNOW_KEY_F5             ::Snow::KeyCode::F5
#define SNOW_KEY_F6             ::Snow::KeyCode::F6
#define SNOW_KEY_F7             ::Snow::KeyCode::F7
#define SNOW_KEY_F8             ::Snow::KeyCode::F8
#define SNOW_KEY_F9             ::Snow::KeyCode::F9
#define SNOW_KEY_F10            ::Snow::KeyCode::F10
#define SNOW_KEY_F11            ::Snow::KeyCode::F11
#define SNOW_KEY_F12            ::Snow::KeyCode::F12
#define SNOW_KEY_F13            ::Snow::KeyCode::F13
#define SNOW_KEY_F14            ::Snow::KeyCode::F14
#define SNOW_KEY_F15            ::Snow::KeyCode::F15
#define SNOW_KEY_F16            ::Snow::KeyCode::F16
#define SNOW_KEY_F17            ::Snow::KeyCode::F17
#define SNOW_KEY_F18            ::Snow::KeyCode::F18
#define SNOW_KEY_F19            ::Snow::KeyCode::F19
#define SNOW_KEY_F20            ::Snow::KeyCode::F20
#define SNOW_KEY_F21            ::Snow::KeyCode::F21
#define SNOW_KEY_F22            ::Snow::KeyCode::F22
#define SNOW_KEY_F23            ::Snow::KeyCode::F23
#define SNOW_KEY_F24            ::Snow::KeyCode::F24
#define SNOW_KEY_F25            ::Snow::KeyCode::F25

#define SNOW_KEY_KP_1           ::Snow::KeyCode:KP1
#define SNOW_KEY_KP_2           ::Snow::KeyCode:KP2
#define SNOW_KEY_KP_3           ::Snow::KeyCode:KP3
#define SNOW_KEY_KP_4           ::Snow::KeyCode:KP4
#define SNOW_KEY_KP_5           ::Snow::KeyCode:KP5
#define SNOW_KEY_KP_6           ::Snow::KeyCode:KP6
#define SNOW_KEY_KP_7           ::Snow::KeyCode:KP7
#define SNOW_KEY_KP_8           ::Snow::KeyCode:KP8
#define SNOW_KEY_KP_9           ::Snow::KeyCode:KP9
#define SNOW_KEY_KP_DECIMAL     ::Snow::KeyCode::KPDecimal
#define SNOW_KEY_KP_DIVIDE      ::Snow::KeyCode::KPDivide
#define SNOW_KEY_KP_MULTIPLY    ::Snow::KeyCode::KPMultiply
#define SNOW_KEY_KP_SUBTRACT    ::Snow::KeyCode::KPSubtract
#define SNOW_KEY_KP_ADD         ::Snow::KeyCode::KPAdd
#define SNOW_KEY_KP_ENTER       ::Snow::KeyCode::KPEnter
#define SNOW_KEY_KP_EQUAL       ::Snow::KeyCode::KPEqual

#define SNOW_KEY_LEFT_SHIFT     ::Snow::KeyCode::LeftShift
#define SNOW_KEY_LEFT_CONTROL   ::Snow::KeyCode::LeftControl
#define SNOW_KEY_LEFT_ALT       ::Snow::KeyCode::LeftAlt
#define SNOW_KEY_LEFT_SUPER     ::Snow::KeyCode::LeftSuper
#define SNOW_KEY_RIGHT_SHIFT    ::Snow::KeyCode::RightShift
#define SNOW_KEY_RIGHT_CONTROL  ::Snow::KeyCode::RightControl
#define SNOW_KEY_RIGHT_ALT      ::Snow::KeyCode::RightAlt
#define SNOW_KEY_RIGHT_SUPER    ::Snow::KeyCode::RightSuper
#define SNOW_KEY_MENU           ::Snow::KeyCode::Menu



#define SNOW_MOUSE_BUTTON_0         ::Snow::MouseCode::Button0
#define SNOW_MOUSE_BUTTON_1         ::Snow::MouseCode::Button1
#define SNOW_MOUSE_BUTTON_2         ::Snow::MouseCode::Button2
#define SNOW_MOUSE_BUTTON_3         ::Snow::MouseCode::Button3
#define SNOW_MOUSE_BUTTON_4         ::Snow::MouseCode::Button4
#define SNOW_MOUSE_BUTTON_5         ::Snow::MouseCode::Button5
#define SNOW_MOUSE_BUTTON_6         ::Snow::MouseCode::Button6
#define SNOW_MOUSE_BUTTON_7         ::Snow::MouseCode::Button7
#define SNOW_MOUSE_BUTTON_LAST      ::Snow::MouseCode::ButtonLast
#define SNOW_MOUSE_BUTTON_LEFT      ::Snow::MouseCode::ButtonLeft
#define SNOW_MOUSE_BUTTON_RIGHT     ::Snow::MouseCode::ButtonRight
#define SNOW_MOUSE_BUTTON_MIDDLE    ::Snow::MouseCode::ButtonMiddle