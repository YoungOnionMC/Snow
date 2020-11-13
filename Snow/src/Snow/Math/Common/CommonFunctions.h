

namespace Snow {
    namespace Math {
        /* Absolute value of the number
        

        */
        template<typename T>
        inline constexpr T abs(T value);

        /* Ceiling of a number

        */
        template<typename T>
        inline constexpr T ceil(T value);

        template<typename T>
        inline constexpr T clamp(T value, T minV, T maxV);

        template<typename T>
        inline constexpr T floor(T value);

        template<typename T>
        inline constexpr T fma(T mul1, T mul2, T add);

        template<typename T>
        inline constexpr T fract(T value);

        template<typename T>
        inline constexpr T frexp(T value, int& exp);

        template<typename T>
        inline constexpr bool isInf(T value);

        template<typename T>
        inline constexpr bool isNan(T value);

        template<typename T>
        inline constexpr T ldexp(T value, int& exp);

        template<typename T>
        inline constexpr T max(T a, T b);

        template<typename T>
        inline constexpr T min(T a, T b);

        template<typename T, typename U>
        inline constexpr T mix(T a, T b, const U& m);

        template<typename T>
        inline constexpr T mod(T value, T mod);

        template<typename T>
        inline constexpr T modf(T value, T& integerPart);

        template<typename T>
        inline constexpr T mul(T value1, T value2);

        template<typename T>
        inline constexpr T round(T value);

        template<typename T>
        inline constexpr T roundEven(T value);

        template<typename T>
        inline constexpr T sign(T value);

        template<typename T>
        inline constexpr T smoothstep(T edge0, T edge1, T value);

        template<typename T>
        inline constexpr T step(T edge, T value);

        template<typename T>
        inline constexpr T trunc(T value);

        // Exponential Functions

        template<typename T>
        inline constexpr T sqrt(T value);

        template<typename T>
        inline constexpr T inversesqrt(T value);

        // Trigonomic functions

        template<typename T>
        inline constexpr T sin(T value);

        template<typename T>
        inline constexpr T cos(T value);

        template<typename T>
        inline constexpr T tan(T value);

        

        template<typename T>
        inline constexpr T radians(T value);

        template<typename T>
        inline constexpr T degrees(T value);
    }
}

#include "Snow/Math/Common/CommonFunctions.inl"