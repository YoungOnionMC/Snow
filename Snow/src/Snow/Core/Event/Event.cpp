#include "spch.h"
#include "Snow/Core/Event/Event.h"

namespace Snow {
    namespace Core {
        namespace Event {
            std::vector<size_t>* BaseEvent::m_EventTypes;

            EventID BaseEvent::RegisterEventType(size_t size) {
                if(m_EventTypes == nullptr)
                    m_EventTypes = new std::vector<size_t>();

                EventID eventID = m_EventTypes->size();
                m_EventTypes->push_back(size);
                return eventID;
            }
        }
    }
}