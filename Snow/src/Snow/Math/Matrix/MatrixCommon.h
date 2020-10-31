#pragma once
#include <cmath>

namespace Snow {
    namespace Math {
        template<size_t R, size_t C, typename T> struct Matrix;

        template<typename T> using tmat2x2 = Matrix<2, 2, T>;
		template<typename T> using tmat2x3 = Matrix<2, 3, T>;
		template<typename T> using tmat2x4 = Matrix<2, 4, T>;
		template<typename T> using tmat3x2 = Matrix<3, 2, T>;
		template<typename T> using tmat3x3 = Matrix<3, 3, T>;
		template<typename T> using tmat3x4 = Matrix<3, 4, T>;
		template<typename T> using tmat4x2 = Matrix<4, 2, T>;
		template<typename T> using tmat4x3 = Matrix<4, 3, T>;
		template<typename T> using tmat4x4 = Matrix<4, 4, T>;

        enum gneType {

        };

        template<typename gentype>
		struct initGenType {
            static gentype identity() {
				return gentype(1);
			}
        };
    }
}