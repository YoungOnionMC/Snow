namespace Snow {
    namespace Math {
        template<typename T>
        inline constexpr T& Vector<3, T>::operator[](Vector<3, T>::lengthType i) {
            switch(i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
            }
        }

        template<typename T>
        inline constexpr const T& Vector<3, T>::operator[](Vector<3, T>::lengthType i) const {
            switch(i) {
                default:
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
            }
        }

        template<typename T>
        inline constexpr Vector<3, T>::Vector() :
            x(0), y(0), z(0) {}

        template<typename T>
        inline constexpr Vector<3, T>::Vector(const Vector<3, T>& vec) :
            x(vec.x), y(vec.y), z(vec.z) {}

        template<typename T>
        inline constexpr Vector<3, T>::Vector(T scalar) :
            x(scalar), y(scalar), z(scalar) {}

        template<typename T>
        inline constexpr Vector<3, T>::Vector(T x, T y, T z) :
            x(x), y(y), z(z) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>::Vector(const Vector<1, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.x)), z(static_cast<T>(vec.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(A x, B y, C z) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(A x, B y, const Vector<1, C>& z) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(A x, const Vector<1, B>& y, C z) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(const Vector<1, A>& x, B y, C z) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<3, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<3, T>::Vector(A x, const Vector<2, B>& yz) :
            x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<3, T>::Vector(const Vector<1, A>& x, const Vector<2, B>& yz) :
            x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<3, T>::Vector(const Vector<2, A>& xy, B z) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<3, T>::Vector(const Vector<2, A>& xy, const Vector<1, B>& z) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>::Vector(const Vector<3, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>::Vector(const Vector<4, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)) {}

        template<typename T>
        inline constexpr Vector<3, T>& Vector<3, T>::operator=(const Vector<3, T>& vec) {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator=(const Vector<3, U>& vec) {
            this->x = static_cast<T>(vec.x);
            this->y = static_cast<T>(vec.y);
            this->z = static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator+=(U value) {
            this->x += static_cast<T>(value);
            this->y += static_cast<T>(value);
            this->z += static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator+=(const Vector<1, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.x);
            this->z += static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator+=(const Vector<3, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.y);
            this->z += static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator-=(U value) {
            this->x -= static_cast<T>(value);
            this->y -= static_cast<T>(value);
            this->z -= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator-=(const Vector<1, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.x);
            this->z -= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator-=(const Vector<3, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.y);
            this->z -= static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator*=(U value) {
            this->x *= static_cast<T>(value);
            this->y *= static_cast<T>(value);
            this->z *= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator*=(const Vector<1, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.x);
            this->z *= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator*=(const Vector<3, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.y);
            this->z *= static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator/=(U value) {
            this->x /= static_cast<T>(value);
            this->y /= static_cast<T>(value);
            this->z /= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator/=(const Vector<1, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.x);
            this->z /= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator/=(const Vector<3, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.y);
            this->z /= static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        inline constexpr Vector<3, T>& Vector<3, T>::operator++() {
            ++this->x;
            ++this->y;
            ++this->z;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<3, T>& Vector<3, T>::operator--() {
            --this->x;
            --this->y;
            --this->z;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<3, T> Vector<3, T>::operator++(int) {
            Vector<3, T> result(*this);
            ++ *this;
            return result;
        }

        template<typename T>
        inline constexpr Vector<3, T> Vector<3, T>::operator--(int) {
            Vector<3, T> result(*this);
            -- *this;
            return result;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator%=(U value) {
            this->x %= static_cast<T>(value);
            this->y %= static_cast<T>(value);
            this->z %= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator%=(const Vector<1, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.x);
            this->z %= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator%=(const Vector<3, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.y);
            this->z %= static_cast<T>(vec.z);
            return *this;
        }

         template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator&=(U value) {
            this->x &= static_cast<T>(value);
            this->y &= static_cast<T>(value);
            this->z &= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator&=(const Vector<1, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.x);
            this->z &= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator&=(const Vector<3, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.y);
            this->z &= static_cast<T>(vec.z);
            return *this;
        }

         template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator|=(U value) {
            this->x |= static_cast<T>(value);
            this->y |= static_cast<T>(value);
            this->z |= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator|=(const Vector<1, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.x);
            this->z |= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator|=(const Vector<3, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.y);
            this->z |= static_cast<T>(vec.z);
            return *this;
        }

         template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator^=(U value) {
            this->x ^= static_cast<T>(value);
            this->y ^= static_cast<T>(value);
            this->z ^= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator^=(const Vector<1, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.x);
            this->z ^= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator^=(const Vector<3, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.y);
            this->z ^= static_cast<T>(vec.z);
            return *this;
        }

         template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator<<=(U value) {
            this->x <<= static_cast<T>(value);
            this->y <<= static_cast<T>(value);
            this->z <<= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator<<=(const Vector<1, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.x);
            this->z <<= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator<<=(const Vector<3, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.y);
            this->z <<= static_cast<T>(vec.z);
            return *this;
        }

         template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator>>=(U value) {
            this->x >>= static_cast<T>(value);
            this->y >>= static_cast<T>(value);
            this->z >>= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator>>=(const Vector<1, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.x);
            this->z >>= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<3, T>& Vector<3, T>::operator>>=(const Vector<3, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.y);
            this->z >>= static_cast<T>(vec.z);
            return *this;
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec) {
            return vec;
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec) {
            return Vector<3, T>(-vec.x, -vec.y, -vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x + value, vec.y + value, vec.z + value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x + vec2.x, vec1.y + vec2.x, vec1.z + vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value + vec.x, value + vec.y, value + vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x + vec2.x, vec1.x + vec2.y, vec1.x + vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator+(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x - value, vec.y - value, vec.z - value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x - vec2.x, vec1.y - vec2.x, vec1.z - vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value - vec.x, value - vec.y, value - vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x - vec2.x, vec1.x - vec2.y, vec1.x - vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator-(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x * value, vec.y * value, vec.z * value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x * vec2.x, vec1.y * vec2.x, vec1.z * vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator*(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value * vec.x, value * vec.y, value * vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x * vec2.x, vec1.x * vec2.y, vec1.x * vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator*(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x / value, vec.y / value, vec.z / value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x / vec2.x, vec1.y / vec2.x, vec1.z / vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator/(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value / vec.x, value / vec.y, value / vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x / vec2.x, vec1.x / vec2.y, vec1.x / vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator/(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x % value, vec.y % value, vec.z % value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x % vec2.x, vec1.y % vec2.x, vec1.z % vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator%(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value % vec.x, value % vec.y, value % vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x % vec2.x, vec1.x % vec2.y, vec1.x % vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator%(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x % vec2.x, vec1.y % vec2.y, vec1.z % vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x & value, vec.y & value, vec.z & value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x & vec2.x, vec1.y & vec2.x, vec1.z & vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value & vec.x, value & vec.y, value & vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x & vec2.x, vec1.x & vec2.y, vec1.x & vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x & vec2.x, vec1.y & vec2.y, vec1.z & vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x | value, vec.y | value, vec.z | value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x | vec2.x, vec1.y | vec2.x, vec1.z | vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator|(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value | vec.x, value | vec.y, value | vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x | vec2.x, vec1.x | vec2.y, vec1.x | vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator|(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x | vec2.x, vec1.y | vec2.y, vec1.z | vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x ^ value, vec.y ^ value, vec.z ^ value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x ^ vec2.x, vec1.y ^ vec2.x, vec1.z ^ vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator^(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value ^ vec.x, value ^ vec.y, value ^ vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x ^ vec2.x, vec1.x ^ vec2.y, vec1.x ^ vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator^(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x ^ vec2.x, vec1.y ^ vec2.y, vec1.z ^ vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x << value, vec.y << value, vec.z << value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x << vec2.x, vec1.y << vec2.x, vec1.z << vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator<<(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value << vec.x, value << vec.y, value << vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x << vec2.x, vec1.x << vec2.y, vec1.x << vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator<<(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x << vec2.x, vec1.y << vec2.y, vec1.z << vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec, T value) {
            return Vector<3, T>(vec.x >> value, vec.y >> value, vec.z >> value);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<3, T>(vec1.x >> vec2.x, vec1.y >> vec2.x, vec1.z >> vec2.x);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator>>(T value, const Vector<3, T>& vec) {
            return Vector<3, T>(value >> vec.x, value >> vec.y, value >> vec.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<1, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x >> vec2.x, vec1.x >> vec2.y, vec1.x >> vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator>>(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x >> vec2.x, vec1.y >> vec2.y, vec1.z >> vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator~(const Vector<3, T>& vec) {
            return Vector<3, T>(~vec.x, ~vec.y, ~vec.z);
        }

        template<typename T>
        inline constexpr bool operator==(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z);
        }

        template<typename T>
        inline constexpr bool operator!=(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return (vec1.x != vec2.x || vec1.y != vec2.y || vec1.z != vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator&&(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x && vec2.x, vec1.y && vec2.y, vec1.z && vec2.z);
        }

        template<typename T>
        inline constexpr Vector<3, T> operator||(const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
            return Vector<3, T>(vec1.x || vec2.x, vec1.y || vec2.y, vec1.z || vec2.z);
        }
    }
}