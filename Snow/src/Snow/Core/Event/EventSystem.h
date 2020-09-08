#pragma once

#include "Snow/Core/Event/Event.h"
#include "Snow/Core/Event/EventListener.h"

#include <list>
#include <map>

namespace Snow {
    namespace Core {
        namespace Event {
            class EventSystem {
            public:
                static void Init();

                static EventSystem& Get() { return *s_Instance; }

                
                static void RegisterClient(EventID eventID, Listener* listener);
                static void UnregisterClient(EventID eventID, Listener* listener);
                static void UnregisterAll(Listener* listener);


                static void SendEvent(Event* event);

                static void ProcessEvents();
                static void ClearEvents();
                static void Shutdown();

                inline static bool IsTypeValid(EventID ID) { return ID < s_RegisteredEventIDs->size(); }


                static uint32_t RegisterEventID();
            private:
                friend class Event;

                static bool AlreadyRegisteredClient(EventID eventID, Listener* listener);
                static void DispatchEvent(Event* event);

                static EventSystem* s_Instance;



                static std::vector<std::pair<EventID, Listener*>> s_Database;

                static std::vector<Event*> s_CurrentEvents;
                static std::vector<uint32_t>* s_RegisteredEventIDs;
            };

            const EventID Event::ID(EventSystem::RegisterEventID());
        }
    }
}