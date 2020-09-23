#pramga once

#include "Snow/Math/Vector/Common.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Vector<4, T> {
            typedef T valueType;
            typedef Vector<4, T> Type;
            typedef Vector<2, bool> boolType;

            typedef size_t lengthType;

            
        }
    }
}