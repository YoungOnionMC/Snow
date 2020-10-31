#include <spch.h>
#include "Snow/Math/Math.h"

namespace Snow {
    namespace Math {
        
        template struct Vector<1, double>;
        template struct Vector<1, float>;
        template struct Vector<1, uint8_t>;
        template struct Vector<1, uint16_t>;
        template struct Vector<1, uint32_t>;
        template struct Vector<1, uint64_t>;
        

        template struct Vector<2, double>;
        template struct Vector<2, float>;
        template struct Vector<2, uint8_t>;
        template struct Vector<2, uint16_t>;
        template struct Vector<2, uint32_t>;
        template struct Vector<2, uint64_t>;

        template struct Vector<3, double>;
        template struct Vector<3, float>;
        template struct Vector<3, uint8_t>;
        template struct Vector<3, uint16_t>;
        template struct Vector<3, uint32_t>;
        template struct Vector<3, uint64_t>;

        template struct Vector<4, double>;
        template struct Vector<4, float>;
        template struct Vector<4, uint8_t>;
        template struct Vector<4, uint16_t>;
        template struct Vector<4, uint32_t>;
        template struct Vector<4, uint64_t>;

        template struct Matrix<4, 4, double>;
        template struct Matrix<4, 4, float>;
        template struct Matrix<4, 4, uint8_t>;
        template struct Matrix<4, 4, uint16_t>;
        template struct Matrix<4, 4, uint32_t>;
        template struct Matrix<4, 4, uint64_t>;
    }
}