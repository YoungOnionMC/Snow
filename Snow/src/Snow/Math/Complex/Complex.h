#pragma once

#include "Snow/Math/Complex/ComplexCommon.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Complex<T> {
            typedef Complex<T> type;
            typedef T value_type;



        };
    }
}