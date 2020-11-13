#pragma once

namespace Snow {
	namespace Math {
		template<typename T>
		typename Matrix<2, 3, T>::row_type& Matrix<2, 3, T>::operator[](typename Matrix<2, 3, T>::row_vec i) {
            assert(i < this->GetColumns());
			return this->values[i];
		}

		template<typename T>
		constexpr const typename Matrix<2, 3, T>::row_type& Matrix<2, 3, T>::operator[](typename Matrix<2, 3, T>::row_vec i) const {
            assert(i < this->GetColumns());
			return this->values[i];
		}

        template<typename T>
        typename Matrix<2, 3, T>::col_type Matrix<2, 3, T>::column(typename Matrix<2, 3, T>::col_vec i) {
            assert(i < this->GetRows());
            return Matrix<2, 3, T>::col_type(this->values[0][i], this->values[1][i]);
        }

        template<typename T>
		constexpr const typename Matrix<2, 3, T>::col_type Matrix<2, 3, T>::column(typename Matrix<2, 3, T>::col_vec i) const {
            assert(i < this->GetRows());
			return Matrix<2, 3, T>::col_type(this->values[0][i], this->values[1][i]);
		}

		template<typename T>
		void Matrix<2, 3, T>::setcolumn(typename Matrix<2, 3, T>::col_vec i, typename Matrix<2, 3, T>::col_type vec) {
			this->values[0][i] = vec.x;
			this->values[1][i] = vec.y;
		}

		template<typename T>
		Matrix<2, 3, T>::Matrix() :
			values { row_type(1, 0, 0), row_type(0, 1, 0) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<2, 3, T>& mat) :
			values { mat[0], mat[1] } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(T scalar) :
			values { row_type(scalar, 0, 0), row_type(0, scalar, 0) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const T& x1, const T& x2, const T& x3,
								   const T& y1, const T& y2, const T& y3) :
			values { row_type(x1, x2, x3), row_type(y1, y2, y3) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const row_type& v1, const row_type& v2) :
			values { v1, v2 } {}

		template<typename T>
		template<typename X1, typename X2, typename X3, typename Y1, typename Y2, typename Y3>
		Matrix<2, 3, T>::Matrix(const X1& x1, const X2& x2, const X3& x3, const Y1& y1, const Y2& y2, const Y3& y3) :
			values { row_type(static_cast<T>(x1), static_cast<T>(x2), static_cast<T>(x3)),
					 row_type(static_cast<T>(y1), static_cast<T>(y2), static_cast<T>(y3))} {}

		template<typename T>
		template<typename V1, typename V2>
		Matrix<2, 3, T>::Matrix(const Vector<3, V1>& v1, const Vector<3, V2>& v2) :
			values{ row_type(v1), row_type(v2) } {}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>::Matrix(const Matrix<2, 3, U>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<2, 2, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<2, 4, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<3, 2, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<3, 3, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<3, 4, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<4, 2, T>& mat) :
			values{ row_type(mat[0], 0), row_type(mat[1], 0) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<4, 3, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}

		template<typename T>
		Matrix<2, 3, T>::Matrix(const Matrix<4, 4, T>& mat) :
			values{ row_type(mat[0]), row_type(mat[1]) } {}


		//unary arithmetic operators

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator=(const Matrix<2, 3, U>& mat) {
			this->values[0] = mat[0];
			this->values[1] = mat[1];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator+=(U scalar) {
			this->value[0] += scalar;
			this->value[1] += scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator+=(const Matrix<2, 3, U>& mat) {
			this->value[0] += mat[0];
			this->value[1] += mat[1];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator-=(U scalar) {
			this->value[0] -= scalar;
			this->value[1] -= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator-=(const Matrix<2, 3, U>& mat) {
			this->value[0] -= mat[0];
			this->value[1] -= mat[1];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator*=(U scalar) {
			this->value[0] *= scalar;
			this->value[1] *= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator*=(const Matrix<2, 3, U>& mat) {
			return (*this = *this * mat);
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator/=(U scalar) {
			this->value[0] /= scalar;
			this->value[1] /= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator/=(const Matrix<2, 3, U>& mat) {
			return *this *= inverse(mat);
		}

		template<typename T>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator++() {
			++this->values[0];
			++this->values[1];
			return *this;
		}

		template<typename T>
		Matrix<2, 3, T>& Matrix<2, 3, T>::operator--() {
			--this->values[0];
			--this->values[1];
			return *this;
		}

		template<typename T>
		Matrix<2, 3, T> Matrix<2, 3, T>::operator++(int) {
			Matrix<2, 3, T> result(*this);
			++ *this;
			return result;
		}

		template<typename T>
		Matrix<2, 3, T> Matrix<2, 3, T>::operator--(int) {
			Matrix<2, 3, T> result(*this);
			-- *this;
			return result;
		}

		//unary operators
		template<typename T>
		Matrix<2, 3, T> operator+(const Matrix<2, 3, T>& mat) {
			return mat;
		}

		template<typename T>
		Matrix<2, 3, T> operator-(const Matrix<2, 3, T>& mat) {
			return Matrix<2, 3, T>(-mat[0], -mat[1]);
		}

		//binary arithmetic operators
		template<typename T>
		Matrix<2, 3, T> operator+(const Matrix<2, 3, T>& mat, T scalar) {
			return Matrix<2, 3, T>(mat[0] + scalar, mat[1] + scalar);
		}

		template<typename T>
		Matrix<2, 3, T> operator+(T scalar, const Matrix<2, 3, T>& mat) {
			return Matrix<2, 3, T>(mat[0] + scalar, mat[1] + scalar);
		}

		template<typename T>
		Matrix<2, 3, T> operator+(const Matrix<2, 3, T>& m1, const Matrix<2, 3, T>& m2) {
			return Matrix<2, 3, T>(m1[0] + m2[0], m1[1] + m2[1]);
		}

		template<typename T>
		Matrix<2, 3, T> operator-(const Matrix<2, 3, T>& mat, T scalar) {
			return Matrix<2, 3, T>(mat[0] - scalar, mat[1] - scalar);
		}

		template<typename T>
		Matrix<2, 3, T> operator-(T scalar, const Matrix<2, 3, T>& mat) {
			return Matrix<2, 3, T>(scalar - mat[0], scalar - mat[1]);
		}

		template<typename T>
		Matrix<2, 3, T> operator-(const Matrix<2, 3, T>& m1, const Matrix<2, 3, T>& m2) {
			return Matrix<2, 3, T>(m1[0] - m2[0], m1[1] - m2[1]);
		}

		template<typename T>
		Matrix<2, 3, T> operator*(const Matrix<2, 3, T>& mat, T scalar) {
			return Matrix<2, 3, T>(mat[0] * scalar, mat[1] * scalar);
		}

		template<typename T>
		Matrix<2, 3, T> operator*(T scalar, const Matrix<2, 3, T>& mat) {
			return Matrix<2, 3, T>(scalar * mat[0], scalar * mat[1]);
		}

		template<typename T>
		typename Matrix<2, 3, T>::col_type operator*(const Matrix<2, 3, T>& mat, const typename Matrix<2, 3, T>::row_type& vec) {
			return Vector<2, T>(mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z, mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z);
		}

		template<typename T>
		typename Matrix<2, 3, T>::row_type operator*(const typename Matrix<2, 3, T>::col_type& vec, const Matrix<2, 3, T>& mat) {
			return Vector<3, T>(vec.x * mat[0][0] + vec.y * mat[1][0], vec.x * mat[0][1] + vec.y * mat[1][1], vec.x * mat[0][2] + vec.y * mat[1][2]);
		}

		template<typename T>
		Matrix<2, 2, T> operator*(const Matrix<2, 3, T>& m1, const Matrix<3, 2, T>& m2) {
			return Matrix<2, 2, T>(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1]));
		}

		template<typename T>
		Matrix<2, 3, T> operator*(const Matrix<2, 3, T>& m1, const Matrix<3, 3, T>& m2) {
			return Matrix<2, 3, T>(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1]), (m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1]), (m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2]));
		}

		template<typename T>
		Matrix<2, 4, T> operator*(const Matrix<2, 3, T>& m1, const Matrix<3, 4, T>& m2) {
			return Matrix<2, 4, T>(
				(m1[0][0] * m2[0][0] + m1[0][1] * m2[1][0] + m1[0][2] * m2[2][0]), (m1[0][0] * m2[0][1] + m1[0][1] * m2[1][1] + m1[0][2] * m2[2][1]), (m1[0][0] * m2[0][2] + m1[0][1] * m2[1][2] + m1[0][2] * m2[2][2]), (m1[0][0] * m2[0][3] + m1[0][1] * m2[1][3] + m1[0][2] * m2[2][3]),
				(m1[1][0] * m2[0][0] + m1[1][1] * m2[1][0] + m1[1][2] * m2[2][0]), (m1[1][0] * m2[0][1] + m1[1][1] * m2[1][1] + m1[1][2] * m2[2][1]), (m1[1][0] * m2[0][2] + m1[1][1] * m2[1][2] + m1[1][2] * m2[2][2]), (m1[1][0] * m2[0][3] + m1[1][1] * m2[1][3] + m1[1][2] * m2[2][3]));
		}

		template<typename T>
		Matrix<2, 3, T> operator/(const Matrix<2, 3, T>& mat, T scalar) {
			return Matrix<2, 3, T>(mat[0] / scalar, mat[1] / scalar);
		}

		template<typename T>
		Matrix<2, 3, T> operator/(T scalar, const Matrix<2, 3, T>& mat) {
			return Matrix<2, 3, T>(scalar / mat[0], scalar / mat[1]);
		}

		//boolean operators

		template<typename T>
		bool operator==(const Matrix<2, 3, T>& m1, const Matrix<2, 3, T>& m2) {
			return (m1[0] == m2[0]) && (m1[1] == m2[1]);
		}

		template<typename T>
		bool operator!=(const Matrix<2, 3, T>& m1, const Matrix<2, 3, T>& m2) {
			return (m1[0] != m2[0]) || (m1[1] != m2[1]);
		}
	}
}