#pragma once

#include "Snow/Math/Matrix/MatrixCommon.h"

namespace Snow {
	namespace Math {
		template<typename T>
		struct Matrix<3, 3, T> {
			typedef T value_type;
			typedef Vector<3, T> row_type;
			typedef Vector<3, T> col_type;
			typedef Matrix<3, 3, T> type;
			typedef Matrix<3, 3, T> transpose_type;

			typedef size_t row_vec;
			static row_vec GetColumns() { return 3; }

			row_type& operator[](row_vec i);
			constexpr const row_type& operator[](row_vec i) const;

            typedef size_t col_vec;
			static col_vec GetRows() { return 3; }

            col_type column(col_vec i);
            constexpr const col_type column(col_vec i) const;

			void setcolumn(col_vec i, col_type vec);

			//implicit constructors
			Matrix();
			Matrix(const Matrix<3, 3, T>& m);

			//explicit constructors
			explicit Matrix(T scalar);
			Matrix(const T& x1, const T& x2, const T& x3,
				   const T& y1, const T& y2, const T& y3,
				   const T& z1, const T& z2, const T& z3);
			Matrix(const row_type& v1, const row_type& v2, const row_type& v3);

			// conversion constructors
			template<typename X1, typename X2, typename X3,
				     typename Y1, typename Y2, typename Y3,
				     typename Z1, typename Z2, typename Z3>
			Matrix(const X1& x1, const X2& x2, const X3& x3, 
				   const Y1& y1, const Y2& y2, const Y3& y3,
				   const Z1& z1, const Z2& z2, const Z3& z3);

			template<typename V1, typename V2, typename V3>
			Matrix(const Vector<3, V1>& v1, const Vector<3, V2>& v2, const Vector<3, V3>& v3);

			//matrix conversions

			template<typename U>
			Matrix(const Matrix<3, 3, U>& m);

			Matrix(const Matrix<2, 2, T>& m);
			Matrix(const Matrix<2, 3, T>& m);
			Matrix(const Matrix<2, 4, T>& m);
			Matrix(const Matrix<3, 2, T>& m);
			Matrix(const Matrix<3, 4, T>& m);
			Matrix(const Matrix<4, 2, T>& m);
			Matrix(const Matrix<4, 3, T>& m);
			Matrix(const Matrix<4, 4, T>& m);

			//unary arithmetic operators

			template<typename U>
			Matrix<3, 3, T>& operator=(const Matrix<3, 3, U>& m);

			template<typename U>
			Matrix<3, 3, T>& operator+=(U scalar);

			template<typename U>
			Matrix<3, 3, T>& operator+=(const Matrix<3, 3, U>& m);

			template<typename U>
			Matrix<3, 3, T>& operator-=(U scalar);

			template<typename U>
			Matrix<3, 3, T>& operator-=(const Matrix<3, 3, U>& m);

			template<typename U>
			Matrix<3, 3, T>& operator*=(U scalar);

			template<typename U>
			Matrix<3, 3, T>& operator*=(const Matrix<3, 3, U>& m);

			template<typename U>
			Matrix<3, 3, T>& operator/=(U scalar);

			template<typename U>
			Matrix<3, 3, T>& operator/=(const Matrix<3, 3, U>& m);

			//Increment and decrement operators
			Matrix<3, 3, T>& operator++();
			Matrix<3, 3, T>& operator--();
			Matrix<3, 3, T> operator++(int);
			Matrix<3, 3, T> operator--(int);

		private:
			row_type values[3];
		};

		//unary operators
		template<typename T>
		Matrix<3, 3, T> operator+(const Matrix<3, 3, T>& m);

		template<typename T>
		Matrix<3, 3, T> operator-(const Matrix<3, 3, T>& m);

		//binary operators

		template<typename T>
		Matrix<3, 3, T> operator+(const Matrix<3, 3, T>& m, T scalar);

		template<typename T>
		Matrix<3, 3, T> operator+(T scalar, const Matrix<3, 3, T>& m);

		template<typename T>
		Matrix<3, 3, T> operator+(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);

		template<typename T>
		Matrix<3, 3, T> operator-(const Matrix<3, 3, T>& m, T scalar);

		template<typename T>
		Matrix<3, 3, T> operator-(T scalar, const Matrix<3, 3, T>& m);

		template<typename T>
		Matrix<3, 3, T> operator-(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);

		template<typename T>
		Matrix<3, 3, T> operator*(const Matrix<3, 3, T>& m, T scalar);

		template<typename T>
		Matrix<3, 3, T> operator*(T scalar, const Matrix<3, 3, T>& m);

		template<typename T>
		typename Matrix<3, 3, T>::col_type operator*(const Matrix<3, 3, T>& m, const typename Matrix<3, 3, T>::row_type& v);

		template<typename T>
		typename Matrix<3, 3, T>::row_type operator*(const typename Matrix<3, 3, T>::col_type& v, const Matrix<3, 3, T>& m);

		template<typename T>
		Matrix<3, 2, T> operator*(const Matrix<3, 3, T>& m1, const Matrix<3, 2, T>& m2);

		template<typename T>
		Matrix<3, 3, T> operator*(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);

		template<typename T>
		Matrix<3, 4, T> operator*(const Matrix<3, 3, T>& m1, const Matrix<3, 4, T>& m2);

		template<typename T>
		Matrix<3, 3, T> operator/(const Matrix<3, 3, T>& m, T scalar);

		template<typename T>
		Matrix<3, 3, T> operator/(T scalar, const Matrix<3, 3, T>& m);

		template<typename T>
		typename Matrix<3, 3, T>::col_type operator/(const Matrix<3, 3, T>& m, const typename Matrix<3, 3, T>::row_type& v);

		template<typename T>
		typename Matrix<3, 3, T>::row_type operator/(const typename Matrix<3, 3, T>::col_type& v, const Matrix<3, 3, T>& m);

		template<typename T>
		Matrix<3, 3, T> operator/(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);

		//Boolean operators

		template<typename T>
		bool operator==(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);

		template<typename T>
		bool operator!=(const Matrix<3, 3, T>& m1, const Matrix<3, 3, T>& m2);



		typedef Matrix<3, 3, float>        Matrix3x3f;
        typedef Matrix<3, 3, double>       Matrix3x3d;
        typedef Matrix<3, 3, bool>         Matrix3x3b;
        typedef Matrix<3, 3, short>        Matrix3x3s;
        typedef Matrix<3, 3, int>          Matrix3x3i;
        typedef Matrix<3, 3, unsigned int> Matrix3x3ui;
        typedef Matrix<3, 3, long>         Matrix3x3l;
        typedef Matrix<3, 3, unsigned long> Matrix3x3ul;
	}
}
#include "Snow/Math/Matrix/Matrix3x3.inl"