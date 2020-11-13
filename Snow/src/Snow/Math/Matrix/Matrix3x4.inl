#pragma once

namespace Snow {
    namespace Math {
        template<typename T>
		typename Matrix<3, 4, T>::row_type& Matrix<3, 4, T>::operator[](typename Matrix<3, 4, T>::row_vec i) {
            assert(i < this->GetColumns());
			return this->values[i];
		}

		template<typename T>
		constexpr const typename Matrix<3, 4, T>::row_type& Matrix<3, 4, T>::operator[](typename Matrix<3, 4, T>::row_vec i) const {
            assert(i < this->GetColumns());
			return this->values[i];
		}

        template<typename T>
        typename Matrix<3, 4, T>::col_type Matrix<3, 4, T>::column(typename Matrix<3, 4, T>::col_vec i) {
            assert(i < this->GetRows());
            return Matrix<3, 4, T>::col_type(this->values[0][i], this->values[1][i], this->values[2][i]);
        }

        template<typename T>
		constexpr const typename Matrix<3, 4, T>::col_type Matrix<3, 4, T>::column(typename Matrix<3, 4, T>::col_vec i) const {
            assert(i < this->GetRows());
			return Matrix<3, 4, T>::col_type(this->values[0][i], this->values[1][i], this->values[3][i]);
		}

		template<typename T>
		void Matrix<3, 4, T>::setcolumn(typename Matrix<3, 4, T>::col_vec i, typename Matrix<3, 4, T>::col_type vec) {
			this->values[0][i] = vec.x;
			this->values[1][i] = vec.y;
            this->values[2][i] = vec.z;
		}

        template<typename T>
        Matrix<3, 4, T>::Matrix() :
            values{ row_type(1, 0, 0, 0), row_type(0, 1, 0, 0), row_type(0, 0, 1, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<3, 4, T>& mat) :
            values{ mat[0], mat[1], mat[2] } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(T scalar) :
            values{ row_type(scalar, 0, 0, 0), row_type(0, scalar, 0, 0), row_type(0, 0, scalar, 0) } {}

        template<typename T>
        template<typename X1, typename X2, typename X3, typename X4,
                 typename Y1, typename Y2, typename Y3, typename Y4,
                 typename Z1, typename Z2, typename Z3, typename Z4>
        Matrix<3, 4, T>::Matrix(const X1& x1, const X2& x2, const X3& x3, const X4& x4,
                                const Y1& y1, const Y2& y2, const Y3& y3, const Y4& y4,
                                const Z1& z1, const Z2& z2, const Z3& z3, const Z4& z4):
            values{ row_type(static_cast<T>(x1), static_cast<T>(x2), static_cast<T>(x3), static_cast<T>(x4)),
                     row_type(static_cast<T>(y1), static_cast<T>(y2), static_cast<T>(y3), static_cast<T>(y4)),
                     row_type(static_cast<T>(z1), static_cast<T>(z2), static_cast<T>(z3), static_cast<T>(z4))} {}

        template<typename T>
        template<typename V1, typename V2, typename V3>
        Matrix<3, 4, T>::Matrix(const Vector<4, V1>& v1, const Vector<4, V2>& v2, const Vector<4, V3>& v3) :
            values{ row_type(v1), row_type(v2), row_type(v3) } {}

        template<typename T>
        template<typename U>
        Matrix<3, 4, T>::Matrix(const Matrix<3, 4, U>& mat) :
            values{ row_type(mat[0]), row_type(mat[1]), row_type(mat[2]) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<2, 2, T>& mat) :
            values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(0, 0, 0, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<2, 3, T>& mat) :
            values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(0, 0, 0, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<2, 4, T>& mat) :
            values{ row_type(mat[0]), row_type(mat[1]), row_type(0, 0, 0, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<3, 2, T>& mat) :
            values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(mat[2], 0, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<3, 3, T>& mat) :
            values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(mat[2], 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<4, 2, T>& mat) :
            values{ row_type(mat[0], 0, 0), row_type(mat[1], 0, 0), row_type(mat[2], 0, 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<4, 3, T>& mat) :
            values{ row_type(mat[0], 0), row_type(mat[1], 0), row_type(mat[2], 0) } {}

        template<typename T>
        Matrix<3, 4, T>::Matrix(const Matrix<4, 4, T>& mat) :
            values{ row_type(mat[0]), row_type(mat[1]), row_type(mat[2]) } {}

        //unary arithmetic operators

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator=(const Matrix<3, 4, U>& mat) {
			this->values[0] = mat[0];
			this->values[1] = mat[1];
			this->values[2] = mat[2];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator+=(U scalar) {
			this->values[0] += scalar;
			this->values[1] += scalar;
			this->values[2] += scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator+=(const Matrix<3, 4, U>& mat) {
			this->values[0] += mat[0];
			this->values[1] += mat[1];
			this->values[2] += mat[2];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator-=(U scalar) {
			this->values[0] -= scalar;
			this->values[1] -= scalar;
			this->values[2] -= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator-=(const Matrix<3, 4, U>& mat) {
			this->values[0] -= mat[0];
			this->values[1] -= mat[1];
			this->values[2] -= mat[2];
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator*=(U scalar) {
			this->values[0] *= scalar;
			this->values[1] *= scalar;
			this->values[2] *= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator*=(const Matrix<3, 4, U>& mat) {
			return(*this = *this * mat);
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator/=(U scalar) {
			this->values[0] /= scalar;
			this->values[1] /= scalar;
			this->values[2] /= scalar;
			return *this;
		}

		template<typename T>
		template<typename U>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator/=(const Matrix<3, 4, U>& mat) {
			return *this *= inverse(mat);
		}

		template<typename T>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator++() {
			++this->values[0];
			++this->values[1];
			++this->values[2];
			return *this;
		}

		template<typename T>
		Matrix<3, 4, T>& Matrix<3, 4, T>::operator--() {
			--this->values[0];
			--this->values[1];
			--this->values[2];
			return *this;
		}

		template<typename T>
		Matrix<3, 4, T> Matrix<3, 4, T>::operator++(int) {
			Matrix<3, 4, T> result(*this);
			++* this;
			return *this;
		}

		template<typename T>
		Matrix<3, 4, T> Matrix<3, 4, T>::operator--(int) {
			Matrix<3, 4, T> result(*this);
			--* this;
			return *this;
		}

        // Unary operators
        template<typename T>
        Matrix<3, 4, T> operator+(const Matrix<3, 4, T>& mat) {
            return mat;
        }

        template<typename T>
        Matrix<3, 4, T> operator-(const Matrix<3, 4, T>& mat) {
            return Matrix<3, 4, T>(-mat[0], -mat[1], -mat[2]);
        }

        // Binary operators
        template<typename T>
		Matrix<3, 4, T> operator+(const Matrix<3, 4, T>& mat, T scalar) {
			return Matrix<3, 4, T>(mat[0] + scalar, mat[1] + scalar, mat[2] + scalar);
		}

		template<typename T>
		Matrix<3, 4, T> operator+(T scalar, const Matrix<3, 4, T>& mat) {
			return Matrix<3, 4, T>(scalar + mat[0], scalar + mat[1], scalar + mat[2]);
		}

		template<typename T>
		Matrix<3, 4, T> operator+(const Matrix<3, 4, T>& m1, const Matrix<3, 4, T>& m2) {
			return Matrix<3, 4, T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]);
		}

		template<typename T>
		Matrix<3, 4, T> operator-(const Matrix<3, 4, T>& mat, T scalar) {
			return Matrix<3, 4, T>(mat[0] - scalar, mat[1] - scalar, mat[2] - scalar);
		}

		template<typename T>
		Matrix<3, 4, T> operator-(T scalar, const Matrix<3, 4, T>& mat) {
			return Matrix<3, 4, T>(scalar - mat[0], scalar - mat[1], scalar - mat[2]);
		}

		template<typename T>
		Matrix<3, 4, T> operator-(const Matrix<3, 4, T>& m1, const Matrix<3, 4, T>& m2) {
			return Matrix<3, 4, T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]);
		}

		template<typename T>
		Matrix<3, 4, T> operator*(const Matrix<3, 4, T>& mat, T scalar) {
			return Matrix<3, 4, T>(mat[0] * scalar, mat[1] * scalar, mat[2] * scalar);
		}

		template<typename T>
		Matrix<3, 4, T> operator*(T scalar, const Matrix<3, 4, T>& mat) {
			return Matrix<3, 4, T>(scalar * mat[0], scalar * mat[1], scalar * mat[2]);
		}

        template<typename T>
        typename Matrix<3, 4, T>::col_type operator*(const Matrix<3, 4, T>& mat, const typename Matrix<3, 4, T>::row_type& vec) {
            
        }
     }
}