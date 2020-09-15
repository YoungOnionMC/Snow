#pragma once

#include <vector>
#include <tuple>

using EventID = uint32_t;

namespace Snow {
    namespace Core {
        namespace Event {

            struct BaseEvent;
            typedef uint32_t (*EventCreateFunction)(std::vector<uint8_t>& memory, BaseEvent* e);
            typedef void (*EventFreeFunction)(BaseEvent* e);

            struct BaseEvent {
            public:
                static uint32_t RegisterEventID(EventCreateFunction createFn, EventFreeFunction freeFn, size_t size);

                inline static EventCreateFunction GetTypeCreateFunction(uint32_t ID) { return std::get<0>((*s_RegisteredEventTypes)[ID]); }
                inline static EventFreeFunction GetTypeFreeFunction(uint32_t ID) { return std::get<1>((*s_RegisteredEventTypes)[ID]); }
                inline static size_t GetTypeSize(uint32_t ID) { return std::get<2>((*s_RegisteredEventTypes)[ID]); }

                inline static bool IsTypeValid(EventID ID) { return ID < s_RegisteredEventTypes->size(); }

            private:
                static std::vector<std::tuple<EventCreateFunction, EventFreeFunction, size_t>>* s_RegisteredEventTypes;
            };
            
            template<typename T>
            struct Event : public BaseEvent {
            protected:
                static const EventCreateFunction CreateFunction;
                static const EventFreeFunction FreeFunction;

            public:
                static const EventID ID;
                static const size_t Size;
            };

            template<typename E>
            uint32_t EventCreate(std::vector<uint8_t>& memory, BaseEvent* e) {
                size_t index = memory.size();
                memory.resize(index + E::Size);
                E* event = new(&memory[index])E(*(E*)e);
                
                return index;
            }

            template<typename E>
            void EventFree(BaseEvent* e) {
                E* event = (E*)e;
                event->~E();
            }

            template<typename T>
            const EventID Event<T>::ID(BaseEvent::RegisterEventID(EventCreate<T>, EventFree<T>, sizeof(T)));

            template<typename T>
            const size_t Event<T>::Size(sizeof(T));

            template<typename T>
            const EventCreateFunction Event<T>::CreateFunction(EventCreate<T>);

            template<typename T>
            const EventFreeFunction Event<T>::FreeFunction(EventFree<T>);
            
        }
    }
}