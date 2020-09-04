#pragma once
#include <string>

using EventID = uint32_t;

namespace Snow {
    namespace Core {
        namespace Event {
            class Event {
            public:
                Event() {}

                ~Event() = default;

                inline const EventID GetEventID() const { return m_EventID; }

            private:
                static const EventID m_EventID;
            };
            
        }
    }
}