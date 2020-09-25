namespace Snow {
    namespace Math {
        template<typename T>
        inline constexpr T& Vector<4, T>::operator[](typename Vector<4, T>::lengthType i) {
            switch(i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            }
        }

        template<typename T>
        inline constexpr const T& Vector<4, T>::operator[](typename Vector<4, T>::lengthType i) const {
            switch(i) {
            default:
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            case 3:
                return w;
            }
        }

        template<typename T>
        inline constexpr Vector<4, T>::Vector() :
            x(0), y(0), z(0), w(0) {}

        template<typename T>
        inline constexpr Vector<4, T>::Vector(const Vector<4, T>& vecec) :
            x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

        template<typename T>
        inline constexpr Vector<4, T>::Vector(T value) :
            x(value), y(value), z(value), w(value) {}

        template<typename T>
        inline constexpr Vector<4, T>::Vector(T x, T y, T z, T w) :
            x(x), y(y), z(z), w(w) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>::Vector(const Vector<1, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.x)), z(static_cast<T>(vec.x)), w(static_cast<T>(vec.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, B y, C z, D w) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, B y, C z, const Vector<1, D>& w) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, B y, const Vector<1, C>& z, D w) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, B y, const Vector<1, C>& z, const Vector<1, D>& w) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<1, B>& y, C z, D w) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<1, B>& y, C z, const Vector<1, D>& w) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z, D w) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<1, B>& y, const Vector<1, C>& z, const Vector<1, D>& w) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, B y, C z, D w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, B y, C z, const Vector<1, D>& w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z, D w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, B y, const Vector<1, C>& z, const Vector<1, D>& w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z, D w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, C z, const Vector<1, D>& w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z, D w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C, typename D>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<1, C>& z, const Vector<1, D>& w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(A x, B y, const Vector<2, C>& zw) :
            x(static_cast<T>(x)), y(static_cast<T>(y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<1, B>& y, const Vector<2, C>& zw) :
            x(static_cast<T>(x)), y(static_cast<T>(y.x)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, B y, const Vector<2, C>& zw) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<1, B>& y, const Vector<2, C>& zw) :
            x(static_cast<T>(x.x)), y(static_cast<T>(y.x)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<2, B>& yz, C w) :
            x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<2, B>& yz, const Vector<1, C>& w) :
            x(static_cast<T>(x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<2, B>& yz, C w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<2, B>& yz, const Vector<1, C>& w) :
            x(static_cast<T>(x.x)), y(static_cast<T>(yz.x)), z(static_cast<T>(yz.y)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<2, A>& xy, B z, C w) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<2, A>& xy, B z, const Vector<1, C>& w) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<2, A>& xy, const Vector<1, B>& z, C w) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B, typename C>
        inline constexpr Vector<4, T>::Vector(const Vector<2, A>& xy, const Vector<1, B>& z, const Vector<1, C>& w) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(z.x)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<4, T>::Vector(A x, const Vector<3, B>& yzw) :
            x(static_cast<T>(x)), y(static_cast<T>(yzw.x)), z(static_cast<T>(yzw.y)), w(static_cast<T>(yzw.z)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<4, T>::Vector(const Vector<1, A>& x, const Vector<3, B>& yzw) :
            x(static_cast<T>(x.x)), y(static_cast<T>(yzw.x)), z(static_cast<T>(yzw.y)), w(static_cast<T>(yzw.z)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<4, T>::Vector(const Vector<3, A>& xyz, B w) :
            x(static_cast<T>(xyz.x)), y(static_cast<T>(xyz.y)), z(static_cast<T>(xyz.z)), w(static_cast<T>(w)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<4, T>::Vector(const Vector<3, A>& xyz, const Vector<1, B>& w) :
            x(static_cast<T>(xyz.x)), y(static_cast<T>(xyz.y)), z(static_cast<T>(xyz.z)), w(static_cast<T>(w.x)) {}

        template<typename T>
        template<typename A, typename B>
        inline constexpr Vector<4, T>::Vector(const Vector<2, A>& xy, const Vector<2, B>& zw) :
            x(static_cast<T>(xy.x)), y(static_cast<T>(xy.y)), z(static_cast<T>(zw.x)), w(static_cast<T>(zw.y)) {}

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>::Vector(const Vector<4, U>& vec) :
            x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y)), z(static_cast<T>(vec.z)), w(static_cast<T>(vec.w)) {}

        template<typename T>
        inline constexpr Vector<4, T>& Vector<4, T>::operator=(const Vector<4, T>& vec) {
            this->x = vec.x;
            this->y = vec.y;
            this->z = vec.z;
            this->w = vec.w;
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator=(const Vector<4, U>& vec) {
            this->x = static_cast<T>(vec.x);
            this->y = static_cast<T>(vec.y);
            this->z = static_cast<T>(vec.z);
            this->w = static_cast<T>(vec.w);
            return *this;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator+=(U value) {
            this->x += static_cast<T>(value);
            this->y += static_cast<T>(value);
            this->z += static_cast<T>(value);
            this->w += static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator+=(const Vector<1, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.x);
            this->z += static_cast<T>(vec.x);
            this->w += static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator+=(const Vector<4, U>& vec) {
            this->x += static_cast<T>(vec.x);
            this->y += static_cast<T>(vec.y);
            this->z += static_cast<T>(vec.z);
            this->w += static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator-=(U value) {
            this->x -= static_cast<T>(value);
            this->y -= static_cast<T>(value);
            this->z -= static_cast<T>(value);
            this->w -= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator-=(const Vector<1, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.x);
            this->z -= static_cast<T>(vec.x);
            this->w -= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator-=(const Vector<4, U>& vec) {
            this->x -= static_cast<T>(vec.x);
            this->y -= static_cast<T>(vec.y);
            this->z -= static_cast<T>(vec.z);
            this->w -= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator*=(U value) {
            this->x *= static_cast<T>(value);
            this->y *= static_cast<T>(value);
            this->z *= static_cast<T>(value);
            this->w *= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator*=(const Vector<1, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.x);
            this->z *= static_cast<T>(vec.x);
            this->w *= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator*=(const Vector<4, U>& vec) {
            this->x *= static_cast<T>(vec.x);
            this->y *= static_cast<T>(vec.y);
            this->z *= static_cast<T>(vec.z);
            this->w *= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator/=(U value) {
            this->x /= static_cast<T>(value);
            this->y /= static_cast<T>(value);
            this->z /= static_cast<T>(value);
            this->w /= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator/=(const Vector<1, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.x);
            this->z /= static_cast<T>(vec.x);
            this->w /= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator/=(const Vector<4, U>& vec) {
            this->x /= static_cast<T>(vec.x);
            this->y /= static_cast<T>(vec.y);
            this->z /= static_cast<T>(vec.z);
            this->w /= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        inline constexpr Vector<4, T>& Vector<4, T>::operator++() {
            ++this->x;
            ++this->y;
            ++this->z;
            ++this->w;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<4, T>& Vector<4, T>::operator--() {
            --this->x;
            --this->y;
            --this->z;
            --this->w;
            return *this;
        }

        template<typename T>
        inline constexpr Vector<4, T> Vector<4, T>::operator++(int) {
            Vector<4, T> result(*this)
            ++ *this;
            return result;
        }

        template<typename T>
        inline constexpr Vector<4, T> Vector<4, T>::operator--(int) {
            Vector<4, T> result(*this)
            -- *this;
            return result;
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator%=(U value) {
            this->x %= static_cast<T>(value);
            this->y %= static_cast<T>(value);
            this->z %= static_cast<T>(value);
            this->w %= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator%=(const Vector<1, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.x);
            this->z %= static_cast<T>(vec.x);
            this->w %= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator%=(const Vector<4, U>& vec) {
            this->x %= static_cast<T>(vec.x);
            this->y %= static_cast<T>(vec.y);
            this->z %= static_cast<T>(vec.z);
            this->w %= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator&=(U value) {
            this->x &= static_cast<T>(value);
            this->y &= static_cast<T>(value);
            this->z &= static_cast<T>(value);
            this->w &= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator&=(const Vector<1, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.x);
            this->z &= static_cast<T>(vec.x);
            this->w &= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator&=(const Vector<4, U>& vec) {
            this->x &= static_cast<T>(vec.x);
            this->y &= static_cast<T>(vec.y);
            this->z &= static_cast<T>(vec.z);
            this->w &= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator|=(U value) {
            this->x |= static_cast<T>(value);
            this->y |= static_cast<T>(value);
            this->z |= static_cast<T>(value);
            this->w |= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator|=(const Vector<1, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.x);
            this->z |= static_cast<T>(vec.x);
            this->w |= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator|=(const Vector<4, U>& vec) {
            this->x |= static_cast<T>(vec.x);
            this->y |= static_cast<T>(vec.y);
            this->z |= static_cast<T>(vec.z);
            this->w |= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator^=(U value) {
            this->x ^= static_cast<T>(value);
            this->y ^= static_cast<T>(value);
            this->z ^= static_cast<T>(value);
            this->w ^= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator^=(const Vector<1, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.x);
            this->z ^= static_cast<T>(vec.x);
            this->w ^= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator^=(const Vector<4, U>& vec) {
            this->x ^= static_cast<T>(vec.x);
            this->y ^= static_cast<T>(vec.y);
            this->z ^= static_cast<T>(vec.z);
            this->w ^= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator<<=(U value) {
            this->x <<= static_cast<T>(value);
            this->y <<= static_cast<T>(value);
            this->z <<= static_cast<T>(value);
            this->w <<= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator<<=(const Vector<1, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.x);
            this->z <<= static_cast<T>(vec.x);
            this->w <<= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator<<=(const Vector<4, U>& vec) {
            this->x <<= static_cast<T>(vec.x);
            this->y <<= static_cast<T>(vec.y);
            this->z <<= static_cast<T>(vec.z);
            this->w <<= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator>>=(U value) {
            this->x >>= static_cast<T>(value);
            this->y >>= static_cast<T>(value);
            this->z >>= static_cast<T>(value);
            this->w >>= static_cast<T>(value);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator>>=(const Vector<1, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.x);
            this->z >>= static_cast<T>(vec.x);
            this->w >>= static_cast<T>(vec.x);
            return *this
        }

        template<typename T>
        template<typename U>
        inline constexpr Vector<4, T>& Vector<4, T>::operator>>=(const Vector<4, U>& vec) {
            this->x >>= static_cast<T>(vec.x);
            this->y >>= static_cast<T>(vec.y);
            this->z >>= static_cast<T>(vec.z);
            this->w >>= static_cast<T>(vec.w);
            return *this
        }

        template<typename T>
        inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec) {
            return vec;
        }

        template<typename T>
        inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec) {
            return Vector<4, T>(-vec.x, -vec.y, -vec.z, -vec.w);
        }

        template<typename T>
		inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x + value, v.y + value, v.z + value, v.w + value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x + v2.x, v1.y + v2.x, v1.z + v2.x, v1.w + v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator+(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value + v.x, value + v.y, value + v.z, value + v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator+(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x + v2.x, v1.x + v2.y, v1.x + v2.z, v1.x + v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator+(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
		}

        template<typename T>
		inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x - value, v.y - value, v.z - value, v.w - value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x - v2.x, v1.y - v2.x, v1.z - v2.x, v1.w - v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator-(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value - v.x, value - v.y, value - v.z, value - v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator-(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x - v2.x, v1.x - v2.y, v1.x - v2.z, v1.x - v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator-(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x * value, v.y * value, v.z * value, v.w * value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x * v2.x, v1.y * v2.x, v1.z * v2.x, v1.w * v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator*(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value * v.x, value * v.y, value * v.z, value * v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator*(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x * v2.x, v1.x * v2.y, v1.x * v2.z, v1.x * v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator*(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x / value, v.y / value, v.z / value, v.w / value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x / v2.x, v1.y / v2.x, v1.z / v2.x, v1.w / v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator/(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value / v.x, value / v.y, value / v.z, value / v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator/(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x / v2.x, v1.x / v2.y, v1.x / v2.z, v1.x / v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator/(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x % value, v.y % value, v.z % value, v.w % value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x % v2.x, v1.y % v2.x, v1.z % v2.x, v1.w % v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator%(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value % v.x, value % v.y, value % v.z, value % v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator%(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x % v2.x, v1.x % v2.y, v1.x % v2.z, v1.x % v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator%(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z, v1.w % v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x & value, v.y & value, v.z & value, v.w & value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x & v2.x, v1.y & v2.x, v1.z & v2.x, v1.w & v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator&(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value & v.x, value & v.y, value & v.z, value & v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator&(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x & v2.x, v1.x & v2.y, v1.x & v2.z, v1.x & v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator&(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x & v2.x, v1.y & v2.y, v1.z & v2.z, v1.w & v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x | value, v.y | value, v.z | value, v.w | value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x | v2.x, v1.y | v2.x, v1.z | v2.x, v1.w | v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator|(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value | v.x, value | v.y, value | v.z, value | v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator|(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x | v2.x, v1.x | v2.y, v1.x | v2.z, v1.x | v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator|(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x | v2.x, v1.y | v2.y, v1.z | v2.z, v1.w | v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x ^ value, v.y ^ value, v.z ^ value, v.w ^ value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x ^ v2.x, v1.y ^ v2.x, v1.z ^ v2.x, v1.w ^ v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator^(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value ^ v.x, value ^ v.y, value ^ v.z, value ^ v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator^(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x ^ v2.x, v1.x ^ v2.y, v1.x ^ v2.z, v1.x ^ v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator^(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x ^ v2.x, v1.y ^ v2.y, v1.z ^ v2.z, v1.w ^ v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x << value, v.y << value, v.z << value, v.w << value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x << v2.x, v1.y << v2.x, v1.z << v2.x, v1.w << v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator<<(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value << v.x, value << v.y, value << v.z, value << v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator<<(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x << v2.x, v1.x << v2.y, v1.x << v2.z, v1.x << v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator<<(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x << v2.x, v1.y << v2.y, v1.z << v2.z, v1.w << v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec, T value) {
			return Vector<4, T>(v.x >> value, v.y >> value, v.z >> value, v.w >> value);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec1, const Vector<1, T>& vec2) {
			return Vector<4, T>(v1.x >> v2.x, v1.y >> v2.x, v1.z >> v2.x, v1.w >> v2.x);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator>>(T value, const Vector<4, T>& vec) {
			return Vector<4, T>(value >> v.x, value >> v.y, value >> v.z, value >> v.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator>>(const Vector<1, T> v1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x >> v2.x, v1.x >> v2.y, v1.x >> v2.z, v1.x >> v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator>>(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, T>(v1.x >> v2.x, v1.y >> v2.y, v1.z >> v2.z, v1.w >> v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, T> operator~(const Vector<4, T>& vec) {
			return Vector<4, T>(~v.x, ~v.y, ~v.z, ~v.w);
		}

		template<typename T>
		inline constexpr bool operator==(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
		}

		template<typename T>
		inline constexpr bool operator!=(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z || v1.w != v2.w;
		}

		template<typename T>
		inline constexpr Vector<4, bool> operator&&(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, bool>(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z, v1.w && v2.w);
		}

		template<typename T>
		inline constexpr Vector<4, bool> operator||(const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
			return Vector<4, bool>(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z, v1.w || v2.w);
		}
    }
}