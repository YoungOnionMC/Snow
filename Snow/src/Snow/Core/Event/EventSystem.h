#pragma once

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Event/EventListener.h"

#include <list>
#include <map>

namespace Snow {
    namespace Core {
        namespace Event {

            typedef uint32_t EventID;
            typedef uint32_t EventIndex;

            class EventSystem {
            public:
                static void Init();

                static EventSystem& Get() { return *s_Instance; }

                template<class E>
                static void AddEvent(E& event) { 
                    BaseEvent* ev = &event;
                    uint32_t evID = E::ID;
                    AddEventInternal(evID, ev); 
                }

                template<class E>
                static void RemoveEvent(E* event) { RemoveEventInternal(E::ID, event); }

                static void AddListener(BaseListener* listener);

                static void ProcessEvents();
                static void ClearEvents();
                static void Shutdown();
            private:

                static void AddEventInternal(EventID eventID, BaseEvent* event);
                static void RemoveEventInternal(EventID eventID, EventIndex eventIndex);
                static EventSystem* s_Instance;

                

                static std::map<EventID, std::vector<uint8_t>> s_Events;
                static std::vector<BaseListener*> s_Listeners;
            };
        }
    }
}