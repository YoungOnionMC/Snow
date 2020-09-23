#include <spch.h>
#include "Snow/Core/Event/EventSystem.h"

namespace Snow {
    namespace Core {
        namespace Event {

            EventSystem* EventSystem::s_Instance = nullptr;
            std::map<EventID, std::vector<uint8_t>> EventSystem::s_Events = {};
            std::vector<BaseListener*> EventSystem::s_Listeners = {};

            void EventSystem::Init() {
                s_Instance = new EventSystem();
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
        }
    }
}