#pragma once

#include "Snow/Core/Event/Event.h"

namespace Snow {
    namespace Core {
        namespace Event {
            class Listener {
            public:
                Listener() = default;

                //virtual ~Listener() = 0;

                virtual void HandleEvent(Event* event) = 0;

            };
        }
    }
}