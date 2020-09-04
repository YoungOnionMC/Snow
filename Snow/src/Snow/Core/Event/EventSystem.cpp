#include <spch.h>
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {
        namespace Event {

            EventSystem* EventSystem::s_Instance = nullptr;

            void EventSystem::Init() {
                s_Instance = new EventSystem();
            }

            void EventSystem::RegisterClient(Event event, Listener* listener) {
                if(!listener || AlreadyRegisteredClient(event, listener)){
                    return;
                }
                m_Database.push_back(std::pair<Event, Listener*>(event, listener));
            }

            void EventSystem::UnregisterClient(Event event, Listener* listener) {
                std::vector<std::pair<Event, Listener*>>::iterator it;
                for(it = m_Database.begin(); it!=m_Database.end(); it++) {
                    if(it->second == listener){
                        m_Database.erase(it);
                    }
                }
            }

            void EventSystem::UnregisterAll(Listener* listener) {
                std::vector<std::pair<Event, Listener*>>::iterator it = m_Database.begin();

                while(it != m_Database.end()) {
                    if(it->second == listener) 
                        it = m_Database.erase(it);
                    else
                        it++;
                }
            }

            void EventSystem::SendEvent(Event& event) {
                m_CurrentEvents.push_back(event);
            }

            void EventSystem::ProcessEvents() {
                SNOW_CORE_TRACE("Processing Events");
                while(m_CurrentEvents.size()) {
                    DispatchEvent(m_CurrentEvents.front());
                    m_CurrentEvents.pop_back();
                }
            }

            void EventSystem::ClearEvents() {
                m_CurrentEvents.clear();
            }

            void EventSystem::Shutdown() {
                m_Database.clear();
                m_CurrentEvents.clear();
            }

            bool EventSystem::AlreadyRegisteredClient(Event event, Listener* listener) {
                std::vector<std::pair<Event, Listener*>>::iterator it;
                for(it = m_Database.begin(); it!=m_Database.end(); it++) {
                    if(it->second == listener) {
                        return true;
                    }
                }
                return false;
            }

            void EventSystem::DispatchEvent(Event& event) {
                for(auto p : m_Database) {
                    SNOW_CORE_TRACE("Iterator Count, {0}", m_Database.size());
                    p.second->HandleEvent(event);
                }
            }
        }
    }
}