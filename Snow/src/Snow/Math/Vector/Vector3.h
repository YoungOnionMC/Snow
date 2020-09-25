#pragma once

#include "Snow/Math/Vector/Common.h"
#include "Snow/Math/Vector/VectorSwizzle.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Vector<3, T> {
            typedef T valueType;
            typedef Vector<3, T> Type;
            typedef Vector<3, bool> boolType;

            SNOW_SWIZZLE_GEN_VECTOR_FROM_VECTOR3(T)

            typedef size_t lengthType;
            inline static constexpr lengthType length() { return 3; }

            inline constexpr T& operator[](lengthType i);
            inline constexpr const T& operator[](lengthType i) const;

            // Implicit constructors
            inline constexpr Vector();
            inline constexpr Vector(const Vector& vec);

             // Explicit constructors
            inline constexpr explicit Vector(T scalar);
            inline constexpr Vector(T x, T y, T z);

            // Conversion constructors
            template<typename U>
            inline constexpr explicit Vector(const Vector<1, U>& vec);

            // Explicit conversion constructors
            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, B y, C z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, B y, const Vector<1, C>& z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, const Vector<1, B>& y, C z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, B y, C z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z);


            template<typename A, typename B>
            inline constexpr Vector(A x, const Vector<2, B>& yz);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<2, B>& yz);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<2, A>& xy, B z);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<2, A>& xy, const Vector<1, B>& z);


            template<typename U>
            inline constexpr Vector(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<4, U>& vec);

            // Unary arithmetic operators
            inline constexpr Vector<3, T>& operator=(const Vector& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator+=(U value);

            template<typename U>
            inline constexpr Vector<3, T>& operator+=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator+=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator-=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator-=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator-=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator*=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator*=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator*=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator/=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator/=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator/=(const Vector<3, U>& vec);

            // Inc & Dec operators
            inline constexpr Vector<3, T>& operator++();
            inline constexpr Vector<3, T>& operator--();
            inline constexpr Vector<3, T> operator++(int);
            inline constexpr Vector<3, T> operator--(int);

            // Unary bit operator
            template<typename U>
            inline constexpr Vector<3, T>& operator%=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator%=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator%=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator&=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator&=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator&=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator|=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator|=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator|=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator^=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator^=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator^=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator<<=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator<<=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator<<=(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator>>=(U val);

            template<typename U>
            inline constexpr Vector<3, T>& operator>>=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<3, T>& operator>>=(const Vector<3, U>& vec);

            union { T x, r, s; };
            union { T y, g, t; };
            union { T z, b, p; };
        };

        // Unary operators
        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec);

        // Binary arithmetic operators
        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator+(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator-(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator*(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator/(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        // Binary bit operators
        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator%(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator&(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator|(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator^(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator<<(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator>>(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<1, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, T> operator~(const Vector<3, T>& vec);

        // Boolean operators
        template<typename T>
        inline constexpr bool operator==(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr bool operator!=(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, bool> operator&&(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        template<typename T>
        inline constexpr Vector<3, bool> operator||(const Vector<3, T>& vec1, const Vector<3, T>& vec2);

        typedef Vector<3, float>        Vector3f;
        typedef Vector<3, double>       Vector3d;
        typedef Vector<3, bool>         Vector3b;
        typedef Vector<3, short>        Vector3s;
        typedef Vector<3, int>          Vector3i;
        typedef Vector<3, unsigned int> Vector3ui;
        typedef Vector<3, long>         Vector3l;
        typedef Vector<3, unsigned long> Vector3ul;
    }
}

#include "Vector3.inl"