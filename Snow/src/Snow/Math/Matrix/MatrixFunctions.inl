#pragma once

namespace Snow {
    namespace Math {
        template<typename T>
        inline constexpr T identity() {
            return initGenType<T>::identity();
        }

        template<typename T>
        inline Matrix<4, 4, T> translate(const Matrix<4, 4, T>& mat, const Vector<3, T>& vec) {
            Matrix<4, 4, T> result(mat);
            result.setcolumn(3, mat.column(0) * vec.x + mat.column(1) * vec.y + mat.column(2) * vec.z + mat.column(3));
            return result;
        }

        template<typename T>
        inline Matrix<4, 4, T> rotate(const Matrix<4, 4, T>& mat, T angle, const Vector<3, T>& vec) {
            const T a = angle;
            const T c = cos(a);
            const T s = sin(a);

            Vector<3, T> axis(normalize(vec));
            Vector<3, T> temp((T(1) - c) * axis);

            Matrix<4, 4, T> rot;
            rot[0][0] = c + temp[0] * axis[0];
            rot[0][1] = temp[1] * axis[0] - s * axis[2];
            rot[0][2] = temp[2] * axis[0] + s * axis[1];

            rot[1][0] = temp[0] * axis[1] + s * axis[2];
            rot[1][1] = c + temp[1] * axis[1];
            rot[1][2] = temp[2] * axis[1] - s * axis[0];

            rot[2][0] = temp[0] * axis[2] - s * axis[1];
            rot[2][1] = temp[1] * axis[2] + s * axis[0];
            rot[2][2] = c + temp[2] * axis[2];

            Matrix<4, 4, T> r;
            r.setcolumn(0, mat.column(0) * rot[0][0] + mat.column(1) * rot[1][0] + mat.column(2) * rot[2][0]);
		    r.setcolumn(1, mat.column(0) * rot[0][1] + mat.column(1) * rot[1][1] + mat.column(2) * rot[2][1]);
		    r.setcolumn(2, mat.column(0) * rot[0][2] + mat.column(1) * rot[1][2] + mat.column(2) * rot[2][2]);
            r.setcolumn(3, mat.column(3));
            return r;
        }

        template<typename T>
        inline Matrix<4, 4, T> scale(const Matrix<4, 4, T>& mat, const Vector<3, T>& vec) {
            Matrix<4, 4, T> result;
            result.setcolumn(0, mat.column(0) * vec[0]);
            result.setcolumn(1, mat.column(1) * vec[1]);
            result.setcolumn(2, mat.column(2) * vec[2]);
            result.setcolumn(3, mat.column(3));
            return result;
        }
    }
}