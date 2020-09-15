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


               // static bool AlreadyRegisteredClient(EventID* eventIDs, Listener* listener);
               // template<class Event>
               // static void DispatchEvent(BaseEvent* event);

                static EventSystem* s_Instance;

                

               // static std::vector<RegisteredClient> s_RegisteredClients;

                static std::map<EventID, std::vector<uint8_t>> s_Events;
                static std::vector<BaseListener*> s_Listeners;
                //static std::vector<BaseEvent*> s_CurrentEvents;
            };
        }
    }
}




/*

void EntityComponentSystem::UpdateSystems(SystemList& systemList, Core::Time::Timestep ts) {
			std::vector<BaseComponent*> componentParam;
			std::vector<std::vector<Math::uint8_t>*> componentArrays;
			for (Math::uint32_t i = 0; i < systemList.GetSize(); i++) {
				const std::vector<Math::uint32_t>& componentTypes = systemList[i]->GetComponentTypes();
				if (componentTypes.size() == 1) {
					size_t typeSize = BaseComponent::GetTypeSize(componentTypes[0]);
					std::vector<uint8_t> array = s_Components[componentTypes[0]];
					for (Math::uint32_t j = 0; j < array.size(); j += typeSize) {
						BaseComponent* comp = (BaseComponent*)&array[j];
						systemList[i]->UpdateComponents(ts, &comp);
					}
				}
			}
		}






*/