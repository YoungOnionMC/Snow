#pragma once
#include "Snow/Math/Vector/Common.h"

namespace Snow {
    namespace Math {
        template<size_t L, typename T>
        inline constexpr Vector<L, T> abs(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> ceil(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> clamp(const Vector<L, T>& vec, T min, T max);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> clamp(const Vector<L, T>& vec, const Vector<L, T>& min, const Vector<L, T>& max);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> floor(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> fract(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> frexp(const Vector<L, T>& vec, const Vector<L, int>& exp);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> isInf(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> isNaN(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> ldexp(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> max(const Vector<L, T>& vec, T value);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> max(T value, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> max(const Vector<L, T>& vec, const Vector<L, T>& max);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> min(const Vector<L, T>& vec, T value);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> min(T value, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> min(const Vector<L, T>& vec, const Vector<L, T>& min);

        template<size_t L, typename T>
        template<typename U>
        inline constexpr Vector<L, T> mix(const Vector<L, T>& vec1, const Vector<L, T>& vec2, U a);

        template<size_t L, typename T, typename U>
        inline constexpr Vector<L, T> mix(const Vector<L, T>& vec1, const Vector<L, T>& vec2, const Vector<L, U>& a);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> mod(const Vector<L, T>& vec, const T& value);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> mod(const Vector<L, T>& vec, const Vector<L, T>& m);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> modf(const Vector<L, T>& vec, const T& value);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> modf(const Vector<L, T>& vec, const Vector<L, T>& m);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> round(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> roundEven(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> sign(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> smoothstep(T edge0, T edge1, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> smoothstep(const Vector<L, T>& edge0, const Vector<L, T>& edge1, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> step(T edge, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> step(const Vector<L, T>& edge, const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> trunc(const Vector<L, T>& vec);

        // Exponential functions
        template<size_t L, typename T>
        inline constexpr Vector<L, T> sqrt(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> inversesqrt(const Vector<L, T>& vec);

        // Geometric functions
        template<typename T>
        inline constexpr Vector<3, T> cross(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<size_t L, typename T>
        inline constexpr T distance(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr T dot(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> faceForward(const Vector<L, T>& normal, const Vector<L, T>& incident, const Vector<L, T>& nRef);

        template<size_t L, typename T>
        inline constexpr T length(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> normalize(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> reflect(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> refract(const Vector<L, T>& vec);

        // Relational functions
        template<size_t L>
        inline constexpr bool any(const Vector<L, bool>& vec);

        template<size_t L>
        inline constexpr bool all(const Vector<L, bool>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator<(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator<=(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator>(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator>=(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator==(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator!=(const Vector<L, T>& vec1, const Vector<L, T>& vec2);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> operator~(const Vector<L, T>& vec);

        // Trigonomic functions
        template<size_t L, typename T>
        inline constexpr Vector<L, T> radians(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> degrees(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> sin(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> cos(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> tan(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> asin(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> acos(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> atan(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> atan2(const Vector<L, T>& yvec, const Vector<L, T>& xvec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> sinh(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> cosh(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> tanh(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> asinh(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> acosh(const Vector<L, T>& vec);

        template<size_t L, typename T>
        inline constexpr Vector<L, T> atanh(const Vector<L, T>& vec);

    }
}
#include "VectorFunctions.inl"