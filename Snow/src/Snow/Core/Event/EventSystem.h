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

                void RegisterClient(Event event, Listener* listener);
                void UnregisterClient(Event event, Listener* listener);
                void UnregisterAll(Listener* listener);


                void SendEvent(Event& event);

                void ProcessEvents();
                void ClearEvents();
                void Shutdown();


            private:
                bool AlreadyRegisteredClient(Event event, Listener* listener);
                void DispatchEvent(Event& event);

                static EventSystem* s_Instance;



                std::vector<std::pair<Event, Listener*>> m_Database;

                std::vector<Event> m_CurrentEvents;

            };
        }
    }
}