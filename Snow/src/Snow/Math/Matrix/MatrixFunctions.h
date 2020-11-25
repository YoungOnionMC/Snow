#pragma once

#include "Snow/Math/Matrix/MatrixCommon.h"

#include "Snow/Math/Matrix/Matrix.h"
#include "Snow/Math/Vector/Vector.h"

#include "Snow/Math/Common/CommonFunctions.h"

namespace Snow {
    namespace Math {
        template<size_t R, size_t C, typename T>
        inline const T* valuePtr(const Matrix<R, C, T>& mat);

        template<typename T>
        inline constexpr T identity();

        // Transformation Functions

        template<typename T>
        inline Matrix<4, 4, T> translate(const Matrix<4, 4, T>& mat, const Vector<3, T>& vec);

        template<typename T>
        inline Matrix<4, 4, T> rotate(const Matrix<4, 4, T>& mat, T angle, const Vector<3, T>& axis);

        template<typename T>
        inline Matrix<4, 4, T> scale(const Matrix<4, 4, T>& mat, const Vector<3, T>& vec);

        // Clipspace Functions

        template<typename T>
        inline Matrix<4, 4, T> ortho(T left, T right, T bottom, T top);

        template<typename T>
        inline Matrix<4, 4, T> ortho(T left, T right, T bottom, T top, T nearP, T farP);

        template<typename T>
        inline Matrix<4, 4, T> perspective(T fovy, T aspect, T nearP, T farP);


    }
}

#include "Snow/Math/Matrix/MatrixFunctions.inl"