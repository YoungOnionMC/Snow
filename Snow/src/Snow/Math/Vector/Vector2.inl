namespace Snow {
    namespace Math {
        template<typename T>
        inline constexpr T& Vector<2, T>::operator[](typename Vector<2, T>::lengthType i) {
            switch(i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            }
        }

        template<typename T>
        inline constexpr const T& Vector<2, T>::operator[](typename Vector<2, T>::lengthType i) const {
            switch(i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            }
        }

        template<typename T>
        inline constexpr Vector<2, T>::Vector() :
            x(0), y(0) {}

        template<typename T>
        inline constexpr Vector<2, T>::Vector(const Vector<2, T>& vec) :
            x(vec.x), y(vec.y) {}

        
        template<typename T>
        inline constexpr Vector<2, T>::Vector(T scalar) :
            x(scalar), y(scalar) {}

        template<typename T>
        inline constexpr Vector<2, T>::Vector(T x, T y) :
            x(x), y(y) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>::Vector(const Vector<1, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.x)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<2, T>::Vector(A x, B y) :
            x(static_cast<T>(x)), y(static_cast<T>(y)) {} 

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<2, T>::Vector(const Vector<1, A>& x, B y) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)) {} 

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<2, T>::Vector(A x, const Vector<1, B>& y) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)) {} 

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<2, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>::Vector(const Vector<2, U>& vec) :
            x(vec.x), y(vec.y) {} 

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>::Vector(const Vector<3, U>& vec) :
            x(vec.x), y(vec.y) {} 

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>::Vector(const Vector<4, U>& vec) :
            x(vec.x), y(vec.y) {}

        template<typename T>
        inline constexpr Vector<2, T>& Vector<2, T>::operator=(const Vector<2, T>& vec) {
            this->x = vec.x;
            this->y = vec.y;
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator=(const Vector<2, U>& vec) {
            this->x = static_cast<T>(vec.x);
            this->y = static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator+=(U value) {
            this->x += static_cast<T>(value);
            this->y += static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator+=(const Vector<1, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator+=(const Vector<2, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator-=(U value) {
            this->x -= static_cast<T>(value);
            this->y -= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator-=(const Vector<1, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator-=(const Vector<2, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator*=(U value) {
            this->x *= static_cast<T>(value);
            this->y *= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator*=(const Vector<1, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator*=(const Vector<2, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator/=(U value) {
            this->x /= static_cast<T>(value);
            this->y /= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator/=(const Vector<1, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator/=(const Vector<2, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        inline constexpr Vector<2, T>& Vector<2, T>::operator++() {
            ++this->x;
            ++this->y;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<2, T>& Vector<2, T>::operator--() {
            --this->x;
            --this->y;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<2, T> Vector<2, T>::operator++(int) {
            Vector<2, T> result(*this);
            ++ *this;
            return result;
        }

        template<typename T>
        inline constexpr Vector<2, T> Vector<2, T>::operator--(int) {
            Vector<2, T> result(*this);
            -- *this;
            return result;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator%=(U value) {
            this->x %= static_cast<T>(value);
            this->y %= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator%=(const Vector<1, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator%=(const Vector<2, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator&=(U value) {
            this->x &= static_cast<T>(value);
            this->y &= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator&=(const Vector<1, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator&=(const Vector<2, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator|=(U value) {
            this->x |= static_cast<T>(value);
            this->y |= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator|=(const Vector<1, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator|=(const Vector<2, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator^=(U value) {
            this->x ^= static_cast<T>(value);
            this->y ^= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator^=(const Vector<1, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator^=(const Vector<2, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator<<=(U value) {
            this->x <<= static_cast<T>(value);
            this->y <<= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator<<=(const Vector<1, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator<<=(const Vector<2, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator>>=(U value) {
            this->x >>= static_cast<T>(value);
            this->y >>= static_cast<T>(value);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator>>=(const Vector<1, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.x);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<2, T>& Vector<2, T>::operator>>=(const Vector<2, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.y);
            return *this;
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec) {
            return vec;
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec) {
            return Vector<2, T>(-vec.x, -vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x + value, vec.y + value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x + vec2.x, vec1.y + vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value + vec.x, value + vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x + vec2.x, vec1.x + vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator+(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x + vec2.x, vec1.y + vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x - value, vec.y - value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x - vec2.x, vec1.y - vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value - vec.x, value - vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x - vec2.x, vec1.x - vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator-(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x - vec2.x, vec1.y - vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x * value, vec.y * value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x * vec2.x, vec1.y * vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator*(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value * vec.x, value * vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x * vec2.x, vec1.x * vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator*(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x * vec2.x, vec1.y * vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x / value, vec.y / value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x / vec2.x, vec1.y / vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator/(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value / vec.x, value / vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x / vec2.x, vec1.x / vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator/(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x / vec2.x, vec1.y / vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x % value, vec.y % value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x % vec2.x, vec1.y % vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator%(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value % vec.x, value % vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x % vec2.x, vec1.x % vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator%(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x % vec2.x, vec1.y % vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x & value, vec.y & value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x & vec2.x, vec1.y & vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value & vec.x, value & vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x & vec2.x, vec1.x & vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x & vec2.x, vec1.y & vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x | value, vec.y | value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x | vec2.x, vec1.y | vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator|(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value | vec.x, value | vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x | vec2.x, vec1.x | vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator|(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x | vec2.x, vec1.y | vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x ^ value, vec.y ^ value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x ^ vec2.x, vec1.y ^ vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator^(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value ^ vec.x, value ^ vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x ^ vec2.x, vec1.x ^ vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator^(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x ^ vec2.x, vec1.y ^ vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x << value, vec.y << value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x << vec2.x, vec1.y << vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator<<(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value << vec.x, value << vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x << vec2.x, vec1.x << vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator<<(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x << vec2.x, vec1.y << vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec, T value) {
            return Vector<2, T>(vec.x >> value, vec.y >> value);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec1, const Vector<1, T>& vec2) {
            return Vector<2, T>(vec1.x >> vec2.x, vec1.y >> vec2.x);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator>>(T value, const Vector<2, T>& vec) {
            return Vector<2, T>(value >> vec.x, value >> vec.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<1, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x >> vec2.x, vec1.x >> vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator>>(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x >> vec2.x, vec1.y >> vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator~(const Vector<2, T>& vec) {
            return Vector<2, T>(~vec.x, ~vec.y);
        }

        template<typename T>
        inline constexpr bool operator==(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return (vec1.x == vec2.x && vec1.y == vec2.y);
        }

        template<typename T>
        inline constexpr bool operator!=(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return (vec1.x != vec2.x || vec1.y != vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator&&(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x && vec2.x, vec1.y && vec2.y);
        }

        template<typename T>
        inline constexpr Vector<2, T> operator||(const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
            return Vector<2, T>(vec1.x || vec2.x, vec1.y || vec2.y);
        }
    }
}