#pragma once
#include <cmath>

namespace Snow {
    namespace Math {
        template<size_t L, typename T> struct Vector;
        template<typename T> using tvec1 = Vector<1, T>;
        template<typename T> using tvec2 = Vector<2, T>;
        template<typename T> using tvec3 = Vector<3, T>;
        template<typename T> using tvec4 = Vector<4, T>;
    }
}