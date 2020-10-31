#pragma once

namespace Snow {
    namespace Math {
        template<typename T>
        typename Matrix<4, 4, T>::row_type& Matrix<4, 4, T>::operator[](typename Matrix<4, 4, T>::row_vec i) {
            return this->values[i];
        }

        template<typename T>
		constexpr const typename Matrix<4, 4, T>::row_type& Matrix<4, 4, T>::operator[](typename Matrix<4, 4, T>::row_vec i) const {
			return this->values[i];
		}

        template<typename T>
        typename Matrix<4, 4, T>::col_type Matrix<4, 4, T>::column(typename Matrix<4, 4, T>::col_vec i) {

            return Matrix<4, 4, T>::col_type(this->values[0][i], this->values[1][i], this->values[2][i], this->values[3][i]);
        }

        template<typename T>
		constexpr const typename Matrix<4, 4, T>::col_type Matrix<4, 4, T>::column(typename Matrix<4, 4, T>::col_vec i) const {
			return Matrix<4, 4, T>::col_type(this->values[0][i], this->values[1][i], this->values[2][i], this->values[3][i]);
		}

		template<typename T>
		void Matrix<4, 4, T>::setcolumn(typename Matrix<4, 4, T>::col_vec i, typename Matrix<4, 4, T>::col_type vec) {
			this->values[0][i] = vec.x;
			this->values[1][i] = vec.y;
			this->values[2][i] = vec.z;
			this->values[3][i] = vec.w;
		}

		template<typename T>
		Matrix<4, 4, T>::Matrix() :
			values{ row_type(1, 0, 0, 0), row_type(0, 1, 0, 0), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<4, 4, T>& mat) :
			values{ mat[0], mat[1], mat[2], mat[3] } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(T scalar) :
			values{ row_type(scalar, 0, 0, 0), row_type(0, scalar, 0, 0), row_type(0, 0, scalar, 0), row_type(0, 0, 0, scalar) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const T& x1, const T& x2, const T& x3, const T& x4,
								   const T& y1, const T& y2, const T& y3, const T& y4,
								   const T& z1, const T& z2, const T& z3, const T& z4,
								   const T& w1, const T& w2, const T& w3, const T& w4) :
			values{ row_type(x1, x2, x3, x4), row_type(y1, y2, y3, y4), row_type(z1, z2, z3, z4), row_type(w1, w2, w3, w4) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const row_type& v1, const row_type& v2, const row_type& v3, const row_type& v4) :
			values{ v1, v2, v3, v4 } {}

		template<typename T>
		template<typename X1, typename X2, typename X3,	typename X4,
				 typename Y1, typename Y2, typename Y3,	typename Y4,
				 typename Z1, typename Z2, typename Z3, typename Z4,
				 typename W1, typename W2, typename W3, typename W4>
		Matrix<4, 4, T>::Matrix(const X1& x1, const X2& x2, const X3& x3, const X4& x4,
								   const Y1& y1, const Y2& y2, const Y3& y3, const Y4& y4,
								   const Z1& z1, const Z2& z2, const Z3& z3, const Z4& z4,
								   const W1& w1, const W2& w2, const W3& w3, const W4& w4) :
			values { row_type(static_cast<T>(x1), static_cast<T>(x2), static_cast<T>(x3), static_cast<T>(x4)),
					 row_type(static_cast<T>(y1), static_cast<T>(y2), static_cast<T>(y3), static_cast<T>(y4)),
					 row_type(static_cast<T>(z1), static_cast<T>(z2), static_cast<T>(z3), static_cast<T>(z4)),
					 row_type(static_cast<T>(w1), static_cast<T>(w2), static_cast<T>(w3), static_cast<T>(w4)) } {}

		template<typename T>
		template<typename V1, typename V2, typename V3, typename V4>
		Matrix<4, 4, T>::Matrix(const Vector<4, V1>& v1, const Vector<4, V2>& v2, const Vector<4, V3>& v3, const Vector<4, V4>& v4) :
			values{ row_type(v1), row_type(v2), row_type(v3), row_type(v4) } {}

		//template<typename T>
		//Matrix<4, 4, T>::Matrix(const Matrix<4, 4, T>& mat) :
		//	values{ row_type(mat[0]), row_type(mat[1]), row_type(mat[2]), row_type(mat[3]) } {}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>::Matrix(const Matrix<4, 4, U>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]), row_type(mat[2]), row_type(mat[3]) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<2, 2, T>& mat) :
			values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<2, 3, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<2, 4, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]), row_type(0, 0, 1, 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<3, 2, T>& mat) :
			values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(mat[2], 0, 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<3, 3, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(mat[2], 0), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<3, 4, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]), row_type(mat[2]), row_type(0, 0, 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<4, 2, T>& mat) :
			values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(mat[2], 1, 0), row_type(mat[3], 0, 1) } {}

		template<typename T>
		Matrix<4, 4, T>::Matrix(const Matrix<4, 3, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(mat[2], 0), row_type(mat[3], 1) } {}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator=(const Matrix<4, 4, U>& mat) {
			this->values[0] = mat[0];
			this->values[1] = mat[1];
			this->values[2] = mat[2];
			this->values[3] = mat[3];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator+=(U scalar) {
			this->values[0] += scalar;
			this->values[1] += scalar;
			this->values[2] += scalar;
			this->values[3] += scalar;
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator+=(const Matrix<4, 4, U>& mat) {
			this->values[0] += mat[0];
			this->values[1] += mat[1];
			this->values[2] += mat[2];
			this->values[3] += mat[3];
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator-=(U scalar) {
			this->values[0] -= scalar;
			this->values[1] -= scalar;
			this->values[2] -= scalar;
			this->values[3] -= scalar;
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator-=(const Matrix<4, 4, U>& mat) {
			this->values[0] -= mat[0];
			this->values[1] -= mat[1];
			this->values[2] -= mat[2];
			this->values[3] -= mat[3];
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator*=(U scalar) {
			this->values[0] *= scalar;
			this->values[1] *= scalar;
			this->values[2] *= scalar;
			this->values[3] *= scalar;
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator*=(const Matrix<4, 4, U>& mat) {
			this->values[0] *= mat[0];
			this->values[1] *= mat[1];
			this->values[2] *= mat[2];
			this->values[3] *= mat[3];
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator/=(U scalar) {
			this->values[0] /= scalar;
			this->values[1] /= scalar;
			this->values[2] /= scalar;
			this->values[3] /= scalar;
			*this;
		}

		template<typename T>
		template<typename U>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator/=(const Matrix<4, 4, U>& mat) {
			this->values[0] /= mat[0];
			this->values[1] /= mat[1];
			this->values[2] /= mat[2];
			this->values[3] /= mat[3];
			*this;
		}

		template<typename T>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator++() {
			++this->values[0];
			++this->values[1];
			++this->values[2];
			++this->values[3];
			return *this;
		}

		template<typename T>
		Matrix<4, 4, T>& Matrix<4, 4, T>::operator--() {
			--this->values[0];
			--this->values[1];
			--this->values[2];
			--this->values[3];
			return *this;
		}

		template<typename T>
		Matrix<4, 4, T> Matrix<4, 4, T>::operator++(int) {
			Matrix<4, 4, T> result(*this);
			++* this;
			return *this;
		}

		template<typename T>
		Matrix<4, 4, T> Matrix<4, 4, T>::operator--(int) {
			Matrix<4, 4, T> result(*this);
			--* this;
			return *this;
		}

		//unary operator
		template<typename T>
		Matrix<4, 4, T> operator+(const Matrix<4, 4, T>& mat) {
			return mat;
		}

		template<typename T>
		Matrix<4, 4, T> operator-(const Matrix<4, 4, T>& mat) {
			return Matrix<4, 4, T>(-mat[0], -mat[1], -mat[2], -mat[3]);
		}

		//binary arithmetic operators
		template<typename T>
		Matrix<4, 4, T> operator+(const Matrix<4, 4, T>& mat, T scalar) {
			return Matrix<4, 4, T>(mat[0] + scalar, mat[1] + scalar, mat[2] + scalar, mat[3] + scalar);
		}

		template<typename T>
		Matrix<4, 4, T> operator+(T scalar, const Matrix<4, 4, T>& mat) {
			return Matrix<4, 4, T>(scalar + mat[0], scalar + mat[1], scalar + mat[2], scalar + mat[3]);
		}

		template<typename T>
		Matrix<4, 4, T> operator+(const Matrix<4, 4, T>& m1, const Matrix<4, 4, T>& m2) {
			return Matrix<4, 4, T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
		}

		template<typename T>
		Matrix<4, 4, T> operator-(const Matrix<4, 4, T>& mat, T scalar) {
			return Matrix<4, 4, T>(mat[0] - scalar, mat[1] - scalar, mat[2] - scalar, mat[3] - scalar);
		}

		template<typename T>
		Matrix<4, 4, T> operator-(T scalar, const Matrix<4, 4, T>& mat) {
			return Matrix<4, 4, T>(scalar - mat[0], scalar - mat[1], scalar - mat[2], scalar - mat[3]);
		}

		template<typename T>
		Matrix<4, 4, T> operator-(const Matrix<4, 4, T>& m1, const Matrix<4, 4, T>& m2) {
			return Matrix<4, 4, T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
		}

		template<typename T>
		Matrix<4, 4, T> operator*(const Matrix<4, 4, T>& mat, T scalar) {
			return Matrix<4, 4, T>(mat[0] * scalar, mat[1] * scalar, mat[2] * scalar, mat[3] * scalar);
		}

		template<typename T>
		Matrix<4, 4, T> operator*(T scalar, const Matrix<4, 4, T>& mat) {
			return Matrix<4, 4, T>(scalar * mat[0], scalar * mat[1], scalar * mat[2], scalar * mat[3]);
		}

		template<typename T>
		typename Matrix<4, 4, T>::col_type operator*(const Matrix<4, 4, T>& mat, const typename Matrix<4, 4, T>::row_type& vec) {
			return Vector<4, T>(mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w, mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w, mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w, mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w);
		}

		template<typename T>
		typename Matrix<4, 4, T>::row_type operator*(const typename Matrix<4, 4, T>::col_type& vec, const Matrix<4, 4, T>& mat) {
			return Vector<4, T>(vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + vec.w * mat[3][0], vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + vec.w * mat[3][1], vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + vec.w * mat[3][2], vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + vec.w * mat[3][3]);
		}

		template<typename T>
		Matrix<4, 2, T> operator*(const Matrix<4, 4, T>& m1, const Matrix<4, 2, T>& m2) {
			Matrix<4, 2, T> result(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1]),
				(m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0]), (m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1]),
				(m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0]), (m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1])
				);
		}

		template<typename T>
		Matrix<4, 3, T> operator*(const Matrix<4, 4, T>& m1, const Matrix<4, 3, T>& m2) {
			Matrix<4, 3, T> result(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1]), (m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1]), (m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2]),
				(m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0]), (m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1]), (m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2]),
				(m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0]), (m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1]), (m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2])
			);
		}

		template<typename T>
		Matrix<4, 4, T> operator*(const Matrix<4, 4, T>& m1, const Matrix<4, 4, T>& m2) {
			Matrix<4, 4, T> result(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0] + m1[0][3] * m2[3][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1] + m1[0][3] * m2[3][1]), (m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2] + m1[0][3] * m2[3][2]), (m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3] + m1[0][3] * m2[3][3]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0] + m1[1][3] * m2[3][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1] + m1[1][3] * m2[3][1]), (m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2] + m1[1][3] * m2[3][2]), (m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3] + m1[1][3] * m2[3][3]),
				(m1[2][0] * m2[0][0] + m1[2][1] * m2[1][0] + m1[2][2] * m2[2][0] + m1[2][3] * m2[3][0]), (m1[2][0] * m2[0][1] + m1[2][1] * m2[1][1] + m1[2][2] * m2[2][1] + m1[2][3] * m2[3][1]), (m1[2][0] * m2[0][2] + m1[2][1] * m2[1][2] + m1[2][2] * m2[2][2] + m1[2][3] * m2[3][2]), (m1[2][0] * m2[0][3] + m1[2][1] * m2[1][3] + m1[2][2] * m2[2][3] + m1[2][3] * m2[3][3]),
				(m1[3][0] * m2[0][0] + m1[3][1] * m2[1][0] + m1[3][2] * m2[2][0] + m1[3][3] * m2[3][0]), (m1[3][0] * m2[0][1] + m1[3][1] * m2[1][1] + m1[3][2] * m2[2][1] + m1[3][3] * m2[3][1]), (m1[3][0] * m2[0][2] + m1[3][1] * m2[1][2] + m1[3][2] * m2[2][2] + m1[3][3] * m2[3][2]), (m1[3][0] * m2[0][3] + m1[3][1] * m2[1][3] + m1[3][2] * m2[2][3] + m1[3][3] * m2[3][3])
				);
			return result;
		}
    }
}