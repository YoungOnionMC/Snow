#include <spch.h>
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {
        namespace Event {

            EventSystem* EventSystem::s_Instance = nullptr;
            std::vector<std::pair<EventID, Listener*>> EventSystem::s_Database = {};

            std::vector<Event*> EventSystem::s_CurrentEvents = {};
            std::vector<uint32_t>* EventSystem::s_RegisteredEventIDs = nullptr;

            void EventSystem::Init() {
                s_Instance = new EventSystem();
            }

            void EventSystem::RegisterClient(EventID eventID, Listener* listener) {

                if(!listener || AlreadyRegisteredClient(eventID, listener)){
                    return;
                }
                s_Database.push_back(std::pair<EventID, Listener*>(eventID, listener));
            }

            void EventSystem::UnregisterClient(EventID event, Listener* listener) {
                

                std::vector<std::pair<EventID, Listener*>>::iterator it;
                for(it = s_Database.begin(); it!=s_Database.end(); it++) {
                    if(it->second == listener){
                        s_Database.erase(it);
                    }
                }
            }

            void EventSystem::UnregisterAll(Listener* listener) {
                std::vector<std::pair<EventID, Listener*>>::iterator it = s_Database.begin();

                while(it != s_Database.end()) {
                    if(it->second == listener) 
                        it = s_Database.erase(it);
                    else
                        it++;
                }
            }

            void EventSystem::SendEvent(Event* event) {
                s_CurrentEvents.push_back(event);
            }

            void EventSystem::ProcessEvents() {
                //SNOW_CORE_TRACE("Processing Events, CurrentEventSize {0}", s_CurrentEvents.size());
                while(s_CurrentEvents.size()) {
                    DispatchEvent(s_CurrentEvents.front());
                    s_CurrentEvents.pop_back();
                    SNOW_CORE_TRACE("Done Processing Event, size is {0}", s_CurrentEvents.size());
                }
            }

            void EventSystem::ClearEvents() {
                s_CurrentEvents.clear();
            }

            void EventSystem::Shutdown() {
                s_Database.clear();
                s_CurrentEvents.clear();
            }

            bool EventSystem::AlreadyRegisteredClient(EventID event, Listener* listener) {
                
                return false;
            }

            void EventSystem::DispatchEvent(Event* event) {
                for(auto p : s_Database) {
                    SNOW_CORE_TRACE("Iterator Count, {0}", s_Database.size());

                    p.second->HandleEvent(event);
                }
            }

            uint32_t EventSystem::RegisterEventID() {
                if(s_RegisteredEventIDs == nullptr)
                    s_RegisteredEventIDs = new std::vector<uint32_t>();
                uint32_t eventID = s_RegisteredEventIDs->size() + 1;

                s_RegisteredEventIDs->push_back(eventID);
                return eventID;
            }
        }
    }
}