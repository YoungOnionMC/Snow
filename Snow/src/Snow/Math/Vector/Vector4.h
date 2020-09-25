#pragma once

#include "Snow/Math/Vector/Common.h"
#include "Snow/Math/Vector/VectorSwizzle.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Vector<4, T> {
            typedef T valueType;
            typedef Vector<4, T> Type;
            typedef Vector<2, bool> boolType;

            SNOW_SWIZZLE_GEN_VECTOR_FROM_VECTOR4(T)

            typedef size_t lengthType;
            inline static constexpr lengthType length() { return 4; }

            inline constexpr T& operator[](lengthType i);
            inline constexpr const T& operator[](lengthType i) const;

            // Implicit constructors
            inline constexpr Vector();
            inline constexpr Vector(const Vector& vec);

             // Explicit constructors
            inline constexpr explicit Vector(T scalar);
            inline constexpr Vector(T x, T y, T z, T w);

            // Conversion constructors
            template<typename U>
            inline constexpr explicit Vector(const Vector<1, U>& vec);

            // Explicit conversion constructors
            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, B y, C z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, B y, C z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, B y, const Vector<1, C>& z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, B y, const Vector<1, C>& z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, const Vector<1, B>& y, C z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, const Vector<1, B>& y, C z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, B y, C z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, B y, C z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z, const Vector<1, D>& w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z, D w);

            template<typename A, typename B, typename C, typename D>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z, const Vector<1, D>& w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, B y, const Vector<2, C>& zw);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, const Vector<1, B>& y, const Vector<2, C>& zw);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, B y, const Vector<2, C>& zw);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<2, C>& zw);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, const Vector<2, B>& yz, C w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(A x, const Vector<2, B>& yz, const Vector<1, C>& w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<2, B>& yz, C w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<2, B>& yz, const Vector<1, C>& w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<2, A>& xy, B z, C w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<2, A>& xy, B z, const Vector<1, C>& w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<2, A>& xy, const Vector<1, B>& z, C w);

            template<typename A, typename B, typename C>
            inline constexpr Vector(const Vector<2, A>& xy, const Vector<1, B>& z, const Vector<1, C>& w);

            template<typename A, typename B>
            inline constexpr Vector(A x, const Vector<3, B>& yzw);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<1, A>& x, const Vector<3, B>& yzw);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<3, A>& xyz, B w);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<3, A>& xyz, const Vector<1, B>& w);

            template<typename A, typename B>
            inline constexpr Vector(const Vector<2, A>& xy, const Vector<2, B>& zw);

            template<typename U>
            inline constexpr Vector(const Vector<4, U>& vec);

            // Unary arithmetic operators
            inline constexpr Vector<4, T>& operator=(const Vector& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator+=(U value);

            template<typename U>
            inline constexpr Vector<4, T>& operator+=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator+=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator-=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator-=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator-=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator*=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator*=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator*=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator/=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator/=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator/=(const Vector<4, U>& vec);

            // Inc & Dec operators
            inline constexpr Vector<4, T>& operator++();
            inline constexpr Vector<4, T>& operator--();
            inline constexpr Vector<4, T> operator++(int);
            inline constexpr Vector<4, T> operator--(int);

            // Unary bit operator
            template<typename U>
            inline constexpr Vector<4, T>& operator%=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator%=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator%=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator&=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator&=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator&=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator|=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator|=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator|=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator^=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator^=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator^=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator<<=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator<<=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator<<=(const Vector<4, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator>>=(U val);

            template<typename U>
            inline constexpr Vector<4, T>& operator>>=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<4, T>& operator>>=(const Vector<4, U>& vec);

            union { T x, r, s; };
            union { T y, g, t; };
            union { T z, b, p; };
            union { T w, a, q; };
        };

        // Unary operators
        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec);

        // Binary arithmetic operators
        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator+(T value, const Vector<3, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator-(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator*(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator*(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator/(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator/(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        // Binary bit operators
        template<typename T>
        inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator%(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator%(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator&(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator&(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator|(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator|(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator^(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator^(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator<<(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator<<(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator>>(T value, const Vector<4, T>& vec);

        template<typename T>
        inline constexpr Vector<4, T> operator>>(const Vector<1, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, T> operator~(const Vector<4, T>& vec);

        // Boolean operators
        template<typename T>
        inline constexpr bool operator==(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr bool operator!=(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, bool> operator&&(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        template<typename T>
        inline constexpr Vector<4, bool> operator||(const Vector<4, T>& vec1, const Vector<4, T>& vec2);

        typedef Vector<4, float>        Vector4f;
        typedef Vector<4, double>       Vector4d;
        typedef Vector<4, bool>         Vector4b;
        typedef Vector<4, short>        Vector4s;
        typedef Vector<4, int>          Vector4i;
        typedef Vector<4, unsigned int> Vector4ui;
        typedef Vector<4, long>         Vector4l;
        typedef Vector<4, unsigned long> Vector4ul;
    }
}
#include "Vector4.inl"