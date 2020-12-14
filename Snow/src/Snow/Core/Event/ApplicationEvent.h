#pragma once

#include "Snow/Core/Event/Event.h"

namespace Snow {
	namespace Core {
		namespace Event {
			class WindowCloseEvent : public Event {
			public:
				WindowCloseEvent() = default;

				EVENT_CLASS_TYPE(WindowClose);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};

			class WindowMovedEvent : public Event {
			public:
				WindowMovedEvent(uint32_t xPos, uint32_t yPos) : m_XPosition(xPos), m_YPosition(yPos) {}

				uint32_t GetXPos() const { return m_XPosition; }
				uint32_t GetYPos() const { return m_YPosition; }

				EVENT_CLASS_TYPE(WindowMoved);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			private:
				uint32_t m_XPosition, m_YPosition;
			};

			class WindowResizeEvent : public Event {
			public:
				WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

				uint32_t GetWidth() const { return m_Width; }
				uint32_t GetHeight() const { return m_Height; }

				EVENT_CLASS_TYPE(WindowResize);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			private:
				uint32_t m_Width, m_Height;
			};

			class WindowFullscreenEvent : public Event {
			public:
				WindowFullscreenEvent() = default;

				EVENT_CLASS_TYPE(WindowFullscreen);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};

			class WindowFocusEvent : public Event {
			public:
				WindowFocusEvent(bool focused) : m_Focused(focused) {}

				bool GetFocused() const { return m_Focused; }

				EVENT_CLASS_TYPE(WindowFullscreen);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);

			private:
				bool m_Focused;
			};

			class WindowMinimizedEvent : public Event {
			public:
				WindowMinimizedEvent() = default;

				EVENT_CLASS_TYPE(WindowMinimized);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};

			class AppTickEvent : public Event {
			public:
				AppTickEvent() = default;

				EVENT_CLASS_TYPE(AppTick);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};

			class AppUpdateEvent : public Event {
			public:
				AppUpdateEvent() = default;

				EVENT_CLASS_TYPE(AppUpdate);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};

			class AppRenderEvent : public Event {
			public:
				AppRenderEvent() = default;

				EVENT_CLASS_TYPE(AppRender);
				EVENT_CLASS_CATEGORY(EventCategoryApplication);
			};
		}
	}
}
