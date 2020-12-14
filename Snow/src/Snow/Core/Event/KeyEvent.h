#pragma once

#include "Snow/Core/Event/Event.h"

#include "Snow/Core/InputCodes.h"

#include <sstream>

namespace Snow {
	namespace Core {
		namespace Event {
			class KeyEvent : public Event {
			protected:
				KeyEvent(KeyCode keycode) :
					m_KeyCode(keycode) {}

				KeyCode m_KeyCode;
			public:
				inline KeyCode GetKeyCode() const { return m_KeyCode; }

				EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
			};

			class KeyPressedEvent : public KeyEvent {

			public:
				KeyPressedEvent(KeyCode keycode, int repeat, int modifiers) :
					KeyEvent(keycode), m_Repeat(repeat), m_Modifiers(modifiers) {}

				inline int GetRepeat() const { return m_Repeat; }
				inline int GetModifiers() const { return m_Modifiers; }
				inline bool IsModifier(int modifier) const { return (bool)(m_Modifiers & modifier); }

				std::string ToString() const override {
					std::stringstream ss;
					ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_Repeat << " repeat count) [" << m_Modifiers << " mod bitfield]";
					return ss.str();
				}

				EVENT_CLASS_TYPE(KeyPressed)

			private:
				int m_Repeat;
				int m_Modifiers;
			};


			class KeyReleasedEvent : public KeyEvent {
			public:
				KeyReleasedEvent(KeyCode keycode) :
					KeyEvent(keycode) {}

				std::string ToString() const override {
					std::stringstream ss;
					ss << "KeyReleasedEvent: " << m_KeyCode;
					return ss.str();
				}

				EVENT_CLASS_TYPE(KeyReleased)
			};

			class KeyTypedEvent : public KeyEvent {
			public:
				KeyTypedEvent(KeyCode keycode, int modifiers) :
					KeyEvent(keycode), m_Modifiers(modifiers) {}

				std::string ToString() const override {
					std::stringstream ss;
					ss << "KeyTypedEvent: " << m_KeyCode << " [" << m_Modifiers << "]";
					return ss.str();
				}

				EVENT_CLASS_TYPE(KeyTyped)

			private:
				int m_Modifiers;
			};
		}
	}
}