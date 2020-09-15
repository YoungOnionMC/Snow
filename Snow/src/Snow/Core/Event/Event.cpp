#include "spch.h"
#include "Snow/Core/Event/Event.h"

namespace Snow {
    namespace Core {
        namespace Event {
            std::vector<std::tuple<EventCreateFunction, EventFreeFunction, size_t>>* BaseEvent::s_RegisteredEventTypes;

            uint32_t BaseEvent::RegisterEventID(EventCreateFunction createFn, EventFreeFunction freeFn, size_t size) {
                if(s_RegisteredEventTypes == nullptr)
                    s_RegisteredEventTypes = new std::vector<std::tuple<EventCreateFunction, EventFreeFunction, size_t>>();
                uint32_t eventID = s_RegisteredEventTypes->size();

                s_RegisteredEventTypes->push_back(std::tuple<EventCreateFunction, EventFreeFunction, size_t>(createFn, freeFn, size));
                return eventID;
            }
        }
    }
}