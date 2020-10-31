
#include <cmath>

namespace Snow {
    namespace Math {
        template<typename T>
        inline constexpr T abs(T value) {
            return std::abs(value);
        }

        template<typename T>
        inline constexpr T ceil(T value) {
            return std::ceil(value);
        }

        template<typename T>
        inline constexpr T clamp(T value, T minV, T maxV) {
            assert(minV < maxV)
            return min(max(value, minV), maxV)
        }

        template<typename T>
        inline constexpr T floor(T value) {
            return std::floor(value);
        }

        template<typename T>
        inline constexpr T fma(T mul1, T mul2, T add) {
            return std::fma(mul1, mul2, add);
        }

        template<typename T>
        inline constexpr T fract(T value) {
            return (value - floor(value));
        }

        template<typename T>
        inline constexpr T frexp(T value, int& exp) {
            return std::frexp(value, &exp);
        }


        // Trigonomic Functions

        template<typename T>
        inline constexpr T sin(T value) {
            return static_cast<T>(std::sin(value));
        }

        template<typename T>
        inline constexpr T cos(T value) {
            return static_cast<T>(std::cos(value));
        }

        template<typename T>
        inline constexpr T tan(T value) {
            return static_cast<T>(std::tan(value));
        }

        template<typename T>
        inline constexpr T radians(T value) {
            return value * static_cast<T>(0.01745329251994329576923690768489)
        }

        template<typename T>
        inline constexpr T degrees(T value) {
            return value * static_cast<T>(57.295779513082320876798154814105);
        }
    }
}