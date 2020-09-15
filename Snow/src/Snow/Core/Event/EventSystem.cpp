#include <spch.h>
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {
        namespace Event {

            EventSystem* EventSystem::s_Instance = nullptr;
            std::map<EventID, std::vector<uint8_t>> EventSystem::s_Events = {};
            std::vector<BaseListener*> EventSystem::s_Listeners = {};

            //std::vector<RegisteredClient> EventSystem::s_RegisteredClients = {};

           // std::vector<BaseEvent*> EventSystem::s_CurrentEvents = {};

            void EventSystem::Init() {
                s_Instance = new EventSystem();
                //s_RegisteredClients = std::vector<RegisteredClient>();
            }

            void EventSystem::AddListener(BaseListener* listener) {
                s_Listeners.push_back(listener);
            }

            void EventSystem::ProcessEvents() {
                for(uint32_t i=0; i < s_Listeners.size(); i++){
                    uint32_t eventType = s_Listeners[i]->GetEventType();

                    size_t typeSize = BaseEvent::GetTypeSize(eventType);
                    std::vector<uint8_t> array = s_Events[eventType];
                    for(uint32_t j=0; j<array.size(); j += typeSize) {
                        BaseEvent* event = (BaseEvent*)&array[j];
                        s_Listeners[i]->HandleEvent(event);
                    }
                }
                ClearEvents();
            }

            void EventSystem::ClearEvents() {
                s_Events.clear();
            }

            void EventSystem::AddEventInternal(EventID eventID, BaseEvent* event) {
                EventCreateFunction createFn = BaseEvent::GetTypeCreateFunction(eventID);
                createFn(s_Events[eventID], event);
            }

            void EventSystem::RemoveEventInternal(EventID eventID, EventIndex eventIndex) {
                std::vector<uint8_t>& array = s_Events[eventID];
                EventFreeFunction freeFn = BaseEvent::GetTypeFreeFunction(eventID);
                size_t typeSize = BaseEvent::GetTypeSize(eventID);
                uint32_t sourceIndex = array.size() - typeSize;

                BaseEvent* sourceEvent = (BaseEvent*)&array[sourceIndex];
                BaseEvent* destEvent = (BaseEvent*)&array[eventIndex];
                freeFn(destEvent);

                if(eventIndex == sourceIndex){
                    array.resize(sourceIndex);
                    return;
                }
            }

            //void EventSystem::RegisterClient(EventID eventID, Listener* listener) {
            //    if(!listener || AlreadyRegisteredClient(eventID, listener)){
            //        return;
            //    }
            //    s_RegisteredClients.push_back(std::pair(eventID, listener));
            //}
//
            //template<class Event>
            //void EventSystem::UnregisterClient(Event event, Listener* listener) {
            //    
//
            //    //std::vector<std::pair<EventID, Listener*>>::iterator it;
            //    //for(it = s_Database.begin(); it!=s_Database.end(); it++) {
            //    //    if(it->second == listener){
            //    //        s_Database.erase(it);
            //    //    }
            //    //}
            //}
//
            //void EventSystem::UnregisterAll(Listener* listener) {
            //    //std::vector<std::pair<EventID, Listener*>>::iterator it = s_Database.begin();
////
            //    //while(it != s_Database.end()) {
            //    //    if(it->second == listener) 
            //    //        it = s_Database.erase(it);
            //    //    else
            //    //        it++;
            //    //}
            //}
            /*
            template<class Event>
            void EventSystem::SendEvent(const Event& event) {
                s_CurrentEvents.push_back(&event);
            }

            void EventSystem::ProcessEvents() {
                //SNOW_CORE_TRACE("Processing Events, CurrentEventSize {0}", s_CurrentEvents.size());

                //
                while(s_CurrentEvents.size()) {
                   // DispatchEvent<>(*s_CurrentEvents.front());
                    s_CurrentEvents.pop_back();
                    SNOW_CORE_TRACE("Done Processing Event, size is {0}", s_CurrentEvents.size());
                }
            }

            
            template<typename T>
				bool Dispatch(EventFn<T> func) {
					if (m_Event.GetEventType() == T::GetStaticType()) {
						m_Event.Handled = func(static_cast<T&>(m_Event));
						return true;
					}
					return false;
				}
            

            void EventSystem::ClearEvents() {
                s_CurrentEvents.clear();
            }

            void EventSystem::Shutdown() {
                //s_Database.clear();
                s_CurrentEvents.clear();
            }

            bool EventSystem::AlreadyRegisteredClient(EventID* event, Listener* listener) {
                
                return false;
            }

            template<class Event>
            void EventSystem::DispatchEvent(BaseEvent* event) {

                for(auto p : s_RegisteredClients) {

                    // check if the listener and eventid are valid
                    p.second->HandleEvent(static_cast<Event&>(*event));
                }

                //for(auto p : s_Database) {
                //    SNOW_CORE_TRACE("Iterator Count, {0}", s_Database.size());
//
                //    p.second->HandleEvent(event);
                //}
            }

            */
        }
    }
}