#pragma once

#include "Snow/Math/Vector/Common.h"
#include "Snow/Math/Vector/VectorSwizzle.h"

namespace Snow {
    namespace Math {
        template<typename T>
        struct Vector<1, T> {
            typedef T valueType;
            typedef Vector<1, T> type;
            typedef Vector<1, bool> boolType;

            typedef size_t lengthType;
            inline static constexpr lengthType length() { return 1; }

            inline constexpr T& operator[](lengthType i);
            inline constexpr const T& operator[](lengthType i) const;

            // Implicit constructors
            inline constexpr Vector();
            inline constexpr Vector(const Vector& vec);

            // Explicit constructors
            inline constexpr explicit Vector(T scalar);

            // Conversion vector constructors
            template<typename U>
            inline constexpr Vector(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<2, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<3, U>& vec);

            template<typename U>
            inline constexpr Vector(const Vector<4, U>& vec);

            // Unary arithmetic operators
            inline constexpr Vector<1, T>& operator=(const Vector& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator+=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator+=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator-=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator-=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator*=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator*=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator/=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator/=(const Vector<1, U>& vec);

            // Inc & Dec operators
            inline constexpr Vector<1, T>& operator++();
            inline constexpr Vector<1, T>& operator--();
            inline constexpr Vector<1, T> operator++(int);
            inline constexpr Vector<1, T> operator--(int);

            // Unary bit operator
            template<typename U>
            inline constexpr Vector<1, T>& operator%=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator%=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator&=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator&=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator|=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator|=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator^=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator^=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator<<=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator<<=(const Vector<1, U>& vec);

            template<typename U>
            inline constexpr Vector<1, T>& operator>>=(U value);

            template<typename U>
            inline constexpr Vector<1, T>& operator>>=(const Vector<1, U>& vec);

            union { T x, r, s; };
        };

        // Unary operators
        template<typename T>
        inline constexpr Vector<1, T> operator+(const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator-(const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator+(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator+(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator+(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator-(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator-(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator-(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator*(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator*(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator*(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator/(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator/(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator/(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        // Binary bit operators
        template<typename T>
        inline constexpr Vector<1, T> operator%(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator%(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator%(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator&(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator&(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator&(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator|(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator|(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator|(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator^(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator^(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator^(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator<<(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator<<(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator<<(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator>>(const Vector<1, T>& vec, T value);

        template<typename T>
        inline constexpr Vector<1, T> operator>>(T value, const Vector<1, T>& vec);

        template<typename T>
        inline constexpr Vector<1, T> operator>>(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, T> operator~(const Vector<1, T>& vec);

        // Boolean operators
        template<typename T>
        inline constexpr bool operator==(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr bool operator!=(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, bool> operator&&(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        template<typename T>
        inline constexpr Vector<1, bool> operator||(const Vector<1, T>& vec1, const Vector<1, T>& vec2);

        typedef Vector<1, float>        Vector1f;
        typedef Vector<1, double>       Vector1d;
        typedef Vector<1, bool>         Vector1b;
        typedef Vector<1, short>        Vector1s;
        typedef Vector<1, int>          Vector1i;
        typedef Vector<1, unsigned int> Vector1ui;
        typedef Vector<1, long>         Vector1l;
        typedef Vector<1, unsigned long> Vector1ul;
    }
}

#include "Vector1.inl"