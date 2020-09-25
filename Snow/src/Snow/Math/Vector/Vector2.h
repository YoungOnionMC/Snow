#pragma once

#include "Snow/Math/Vector/Common.h"
#include "Snow/Math/Vector/VectorSwizzle.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Vector<2, T> {
            typedef T valueType;
            typedef Vector<2, T> Type;
            typedef Vector<2, bool> boolType;

            SNOW_SWIZZLE_GEN_VECTOR_FROM_VECTOR2(T)

            typedef size_t lengthType;
            inline static constexpr lengthType length() { return 2; }

            inline constexpr T& operator[](lengthType i);
            inline constexpr const T& operator[](lengthType i) const;

            // Implicit constructors
            inline constexpr Vector();
            inline constexpr Vector(const Vector& vec);
            //inline constexpr Vector(const Vector<2, T>& vec); when we have precision

            // Explicit constructors
            inline constexpr explicit Vector(T scalar);
            inline constexpr Vector(T x, T y);

            // Conversion constructors
            template<typename U>
            inline constexpr explicit Vector(const Vector<1, U>& vec);

            // Explicit conversion constructors
            template<typename A, typename B>
            inline constexpr Vector(A x, B y);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<1, A>& x, B y);

            template<typename A, typename B>
            inline constexpr Vector(A x, const Vector<1, B>& y);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y);

            // Conversion vector constructors
            template<typename U>
            inline constexpr Vector(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<4, U>& vec);

            // Unary arithmetic operators
            inline constexpr Vector<2, T>& operator=(const Vector& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator+=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator+=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator+=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator-=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator-=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator-=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator*=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator*=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator*=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator/=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator/=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator/=(const Vector<2, U>& vec);

            // Inc & Dec operators
            inline constexpr Vector<2, T>& operator++();
            inline constexpr Vector<2, T>& operator--();
            inline constexpr Vector<2, T> operator++(int);
            inline constexpr Vector<2, T> operator--(int);

            // Unary bit operator
            template<typename U>
            inline constexpr Vector<2, T>& operator%=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator%=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator%=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator&=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator&=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator&=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator|=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator|=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator|=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator^=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator^=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator^=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator<<=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator<<=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator<<=(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator>>=(U value);

            template<typename U>
            inline constexpr Vector<2, T>& operator>>=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<2, T>& operator>>=(const Vector<2, U>& vec);

            union { T x, r, s; };
            union { T y, g, t; };
        };

        // Unary operators
        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec);

        // Binary arithmetic operators
        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator+(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator-(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator*(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator/(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        // Binary bit operators
        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator%(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator&(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator|(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator^(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator<<(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator>>(T value, const Vector<2, T>& vec);

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<1, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, T> operator~(const Vector<2, T>& vec);

        // Boolean operators
        template<typename T>
        inline constexpr bool operator==(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr bool operator!=(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, bool> operator&&(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        template<typename T>
        inline constexpr Vector<2, bool> operator||(const Vector<2, T>& vec1, const Vector<2, T>& vec2);

        typedef Vector<2, float>        Vector2f;
        typedef Vector<2, double>       Vector2d;
        typedef Vector<2, bool>         Vector2b;
        typedef Vector<2, short>        Vector2s;
        typedef Vector<2, int>          Vector2i;
        typedef Vector<2, unsigned int> Vector2ui;
        typedef Vector<2, long>         Vector2l;
        typedef Vector<2, unsigned long> Vector2ul;
    }
}

#include "Vector2.inl"