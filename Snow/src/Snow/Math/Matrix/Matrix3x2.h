#pragma once

#include "Snow/Math/Matrix/MatrixCommon.h"

namespace Snow {
	namespace Math {
		template<typename T>
		struct Matrix<3, 2, T> {
			typedef T value_type;
			typedef Vector<2, T> row_type;
			typedef Vector<3, T> col_type;
			typedef Matrix<3, 2, T> type;
			typedef Matrix<2, 3, T> transpose_type;

			typedef size_t row_vec;
			static row_vec GetColumns() { return 2; }

			row_type& operator[](row_vec i);
			constexpr const row_type& operator[](row_vec i) const;

            typedef size_t col_vec;
			static col_vec GetRows() { return 3; }

            col_type column(col_vec i);
            constexpr const col_type column(col_vec i) const;

			void setcolumn(col_vec i, col_type vec);

			//implicit constructors
			Matrix();
			Matrix(const Matrix<3, 2, T>& m);

			//explicit constructors
			explicit Matrix(T scalar);
			Matrix(const T& x1, const T& x2,
				   const T& y1, const T& y2,
				   const T& z1, const T& z2);
			Matrix(const row_type& v1, const row_type& v2, const row_type& v3);

			// conversion constructors
			template<typename X1, typename X2,
					 typename Y1, typename Y2,
					 typename Z1, typename Z2>
			Matrix(const X1& x1, const X2& x2, const Y1& y1, const Y2& y2, const Z1& z1, const Z2& z2);

			template<typename V1, typename V2, typename V3>
			Matrix(const Vector<2, V1>& v1, const Vector<2, V2>& v2, const Vector<2, V3>& v3);

			//matrix conversions

			template<typename U>
			Matrix(const Matrix<3, 2, U>& m);

			Matrix(const Matrix<2, 2, T>& m);
			Matrix(const Matrix<2, 3, T>& m);
			Matrix(const Matrix<2, 4, T>& m);
			Matrix(const Matrix<3, 3, T>& m);
			Matrix(const Matrix<3, 4, T>& m);
			Matrix(const Matrix<4, 2, T>& m);
			Matrix(const Matrix<4, 3, T>& m);
			Matrix(const Matrix<4, 4, T>& m);

			//unary arithmetic operators

			template<typename U>
			Matrix<3, 2, T>& operator=(const Matrix<3, 2, U>& m);

			template<typename U>
			Matrix<3, 2, T>& operator+=(U scalar);

			template<typename U>
			Matrix<3, 2, T>& operator+=(const Matrix<3, 2, U>& m);

			template<typename U>
			Matrix<3, 2, T>& operator-=(U scalar);

			template<typename U>
			Matrix<3, 2, T>& operator-=(const Matrix<3, 2, U>& m);

			template<typename U>
			Matrix<3, 2, T>& operator*=(U scalar);

			template<typename U>
			Matrix<3, 2, T>& operator*=(const Matrix<3, 2, U>& m);

			template<typename U>
			Matrix<3, 2, T>& operator/=(U scalar);

			template<typename U>
			Matrix<3, 2, T>& operator/=(const Matrix<3, 2, U>& m);

			//Increment and decrement operators
			Matrix<3, 2, T>& operator++();
			Matrix<3, 2, T>& operator--();
			Matrix<3, 2, T> operator++(int);
			Matrix<3, 2, T> operator--(int);

		private:
			row_type values[3];
		};

		//unary operators
		template<typename T>
		Matrix<3, 2, T> operator+(const Matrix<3, 2, T>& m);

		template<typename T>
		Matrix<3, 2, T> operator-(const Matrix<3, 2, T>& m);

		//binary operators

		template<typename T>
		Matrix<3, 2, T> operator+(const Matrix<3, 2, T>& m, T scalar);

		template<typename T>
		Matrix<3, 2, T> operator+(T scalar, const Matrix<3, 2, T>& m);

		template<typename T>
		Matrix<3, 2, T> operator+(const Matrix<3, 2, T>& m1, const Matrix<3, 2, T>& m2);

		template<typename T>
		Matrix<3, 2, T> operator-(const Matrix<3, 2, T>& m, T scalar);

		template<typename T>
		Matrix<3, 2, T> operator-(T scalar, const Matrix<3, 2, T>& m);

		template<typename T>
		Matrix<3, 2, T> operator-(const Matrix<3, 2, T>& m1, const Matrix<3, 2, T>& m2);

		template<typename T>
		Matrix<3, 2, T> operator*(const Matrix<3, 2, T>& m, T scalar);

		template<typename T>
		Matrix<3, 2, T> operator*(T scalar, const Matrix<3, 2, T>& m);

		template<typename T>
		typename Matrix<3, 2, T>::col_type operator*(const Matrix<3, 2, T>& m, const typename Matrix<3, 2, T>::row_type& v);

		template<typename T>
		typename Matrix<3, 2, T>::row_type operator*(const typename Matrix<3, 2, T>::col_type& v, const Matrix<3, 2, T>& m);

		template<typename T>
		Matrix<3, 2, T> operator*(const Matrix<3, 2, T>& m1, const Matrix<2, 2, T>& m2);

		template<typename T>
		Matrix<3, 3, T> operator*(const Matrix<3, 2, T>& m1, const Matrix<2, 3, T>& m2);

		template<typename T>
		Matrix<3, 4, T> operator*(const Matrix<3, 2, T>& m1, const Matrix<2, 4, T>& m2);

		template<typename T>
		Matrix<3, 2, T> operator/(const Matrix<3, 2, T>& m, T scalar);

		template<typename T>
		Matrix<3, 2, T> operator/(T scalar, const Matrix<3, 2, T>& m);

		//Boolean operators

		template<typename T>
		bool operator==(const Matrix<3, 2, T>& m1, const Matrix<3, 2, T>& m2);

		template<typename T>
		bool operator!=(const Matrix<3, 2, T>& m1, const Matrix<3, 2, T>& m2);


		typedef Matrix<3, 2, float>        Matrix3x2f;
        typedef Matrix<3, 2, double>       Matrix3x2d;
        typedef Matrix<3, 2, bool>         Matrix3x2b;
        typedef Matrix<3, 2, short>        Matrix3x2s;
        typedef Matrix<3, 2, int>          Matrix3x2i;
        typedef Matrix<3, 2, unsigned int> Matrix3x2ui;
        typedef Matrix<3, 2, long>         Matrix3x2l;
        typedef Matrix<3, 2, unsigned long> Matrix3x2ul;
	}
}
#include "Snow/Math/Matrix/Matrix3x2.inl"