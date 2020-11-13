#pragma once
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
            assert(minV < maxV);
            return min(max(value, minV), maxV);
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

        template<typename T>
        inline constexpr T isInf(T value) {
            return std::isinf(value);
        }

        template<typename T>
        inline constexpr T isNan(T value) {
            return std::isnan(value);
        }

        template<typename T>
        inline constexpr T max(T a, T b) {
            return (a >= b) ? a : b;
        }

        template<typename T>
        inline constexpr T min(T a, T b) {
            return (a < b) ? a : b;
        }

        template<typename T, typename U>
        inline constexpr T mix(T a , T b, const U& m) {
            return static_cast<T>(static_cast<U>(a) * (static_cast<U>(1) - m) + static_cast<U>(b) * m);_
        }

        template<typename T>
        inline constexpr T mix(T a, T b, const bool& m) {
            return m ? b : a;
        }

        template<typename T>
        inline constexpr T mod(T value, T mod) {
            return a - b * floor(a / b);
        }

        template<typename T>
        inline constexpr T modf(T value, int& integerPart) {
            return std::modf(value, integerPart);
        }

        template<typename T>
        inline constexpr T mul(T value1, T value2) {
            return value1 * value2;
        }

        template<typename T>
        inline constexpr T round(T value) {
            return x < static_cast<T>(0) ? static_cast<T>(int(x - static_cast<T>(0.5))) : static_cast<T>(int(value + static_cast<T>(0.5)));
        }

        template<typename T>
        inline constexpr T roundEven(T value) {
            int i = static_cast<int>(value);
            T intPart = static_cast<T>(i);
            T fracPart = fract(value);

            if(fracPart > static_cast<T>(0.5) || fracPart < static_cast<T>(0.5))
                return round(value);
            else if((i % 2) == 0)
                return i;
            else if(value<=static_cast<T>(0))
                return i - static_cast<T>(1);
            else
                return i + static_cast<T>(1);
        }

        template<typename T>
        inline constexpr T sign(T value) {
            return value > static_cast<T>(0) ? static_cast<T>(1) : (value < static_cast<T>(0) ? static_cast<T>(-1) : static_cast<T>(0));
        }

        template<typename T>
        inline constexpr T smoothstep(T edge0, T edge1, T value) {
            const T tmp(clamp((value - edge0) / (edge1 - edge0), T(0), T(1));
            return tmp * tmp * (T(3) - T(2) * tmp);
        }

        template<typename T>
        inline constexpr T step(T edge, T value) {
            return mix(static_cast<T>(1), static_cast<T>(0), x < edge);
        }

        template<typename T>
        inline constexpr T trunc(T value) {
            return value < static_cast<T>(0) ? -std::floor(-x) : std::floor(x);
        }

        // Exponential Functions
        template<typename T>
        inline constexpr T sqrt(T value) {
            return std::sqrt(value);
        }

        template<typename T>
        inline constexpr T inversesqrt(T value) {
            return static_cast<T>(1) / std::sqrt(value);
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
            return value * static_cast<T>(0.01745329251994329576923690768489);
        }

        template<typename T>
        inline constexpr T degrees(T value) {
            return value * static_cast<T>(57.295779513082320876798154814105);
        }
    }
}