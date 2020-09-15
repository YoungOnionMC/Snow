#pragma once

#include "Snow/Core/Event/Event.h"

#include "Snow/Core/Log.h"

namespace Snow {
    namespace Core {
        namespace Event {


            class BaseListener {
            public:
                BaseListener() {}

                virtual void HandleEvent(BaseEvent* event) = 0;

                EventID GetEventType() { return m_EventType; }

            protected:
                void SetEventType(uint32_t eventType) {
                    if(m_EventType!=0)
                        SNOW_CORE_TRACE("EventType already set"); // going to be an assert
                    
                    m_EventType = eventType;
                }
            private:
                EventID m_EventType = 0;
            };
        }
    }
}