
#include "Snow/Math/Common/CommonFunctions.h"
#include "Snow/Math/Vector/Common.h"

namespace Snow {
    namespace Math {
        template<template<size_t L, typename T> class Vector, size_t L, typename R, typename T>
        struct functor1{};

        template<template<size_t L, typename T> class Vector, typename R, typename T>
        struct functor1<Vector, 1, R, T> {
            inline constexpr static Vector<1, R> call(R(*func) (T x), const Vector<1, T>& vec) {
                return Vector<1, R>(func(vec.x));
            }
        };

        template<template<size_t L, typename T> class Vector, typename R, typename T>
        struct functor1<Vector, 2, R, T> {
            inline constexpr static Vector<2, R> call(R(*func) (T x), const Vector<2, T>& vec) {
                return Vector<2, R>(func(vec.x), func(vec.y));
            }
        };

        template<template<size_t L, typename T> class Vector, typename R, typename T>
        struct functor1<Vector, 3, R, T> {
            inline constexpr static Vector<3, R> call(R(*func) (T x), const Vector<3, T>& vec) {
                return Vector<3, R>(func(vec.x), func(vec.y), func(vec.z));
            }
        };

        template<template<size_t L, typename T> class Vector, typename R, typename T>
        struct functor1<Vector, 4, R, T> {
            inline constexpr static Vector<4, R> call(R(*func) (T x), const Vector<4, T>& vec) {
                return Vector<4, R>(func(vec.x), func(vec.y), func(vec.z), func(vec.w));
            }
        };

        template<template<size_t L, typename T> class Vector, size_t L, typename T>
        struct functor2{};

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor2<Vector, 1, T> {
            inline constexpr static Vector<1, T> call(T(*func) (T x, T y), const Vector<1, T>& vec1, const Vector<1, T>& vec2) {
                return Vector<1, T>(func(vec1.x, vec2.x));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor2<Vector, 2, T> {
            inline constexpr static Vector<2, T> call(T(*func) (T x, T y), const Vector<2, T>& vec1, const Vector<2, T>& vec2) {
                return Vector<2, T>(func(vec1.x, vec2.x), func(vec1.y, vec2.y));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor2<Vector, 3, T> {
            inline constexpr static Vector<3, T> call(T(*func) (T x, T y), const Vector<3, T>& vec1, const Vector<3, T>& vec2) {
                return Vector<3, T>(func(vec1.x, vec2.x), func(vec1.y, vec2.y), func(vec1.z, vec2.z));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor2<Vector, 4, T> {
            inline constexpr static Vector<4, T> call(T(*func) (T x, T y), const Vector<4, T>& vec1, const Vector<4, T>& vec2) {
                return Vector<4, T>(func(vec1.x, vec2.x), func(vec1.y, vec2.y), func(vec1.z, vec2.z), func(vec1.w, vec2.w));
            }
        };

        template<template<size_t L, typename T> class Vector, size_t L, typename T>
        struct functor3{};

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor3<Vector, 1, T> {
            inline constexpr static Vector<1, T> call(T(*func) (T x, T y, T z), const Vector<1, T>& vec1, const Vector<1, T>& vec2, const Vector<1, T>& vec3) {
                return Vector<1, T>(func(vec1.x, vec2.x, vec3.x));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor3<Vector, 2, T> {
            inline constexpr static Vector<2, T> call(T(*func) (T x, T y, T z), const Vector<2, T>& vec1, const Vector<2, T>& vec2, const Vector<2, T>& vec3) {
                return Vector<2, T>(func(vec1.x, vec2.x, vec3.x), func(vec1.y, vec2.y, vec3.y));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor3<Vector, 3, T> {
            inline constexpr static Vector<3, T> call(T(*func) (T x, T y, T z), const Vector<3, T>& vec1, const Vector<3, T>& vec2, const Vector<3, T>& vec3) {
                return Vector<3, T>(func(vec1.x, vec2.x, vec3.x), func(vec1.y, vec2.y, vec3.y), func(vec1.z, vec2.z, vec3.z));
            }
        };

        template<template<size_t L, typename T> class Vector, typename T>
        struct functor3<Vector, 4, T> {
            inline constexpr static Vector<4, T> call(T(*func) (T x, T y, T z), const Vector<4, T>& vec1, const Vector<4, T>& vec2, const Vector<4, T>& vec3) {
                return Vector<4, T>(func(vec1.x, vec2.x, vec3.x), func(vec1.y, vec2.y, vec3.y), func(vec1.z, vec2.z, vec3.z), func(vec1.w, vec2.w, vec3.w));
            }
        };



        template<size_t L, typename T>
        inline constexpr Vector<L, T> abs(const Vector<L, T>& vec) {
            return functor1<Vector, L, T, T>::call(abs, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> ceil(const Vector<L, T>& vec) {
            return functor1<Vector, L, T, T>::call(ceil, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> clamp(const Vector<L, T>& vec, T minVec, T maxVec) {
            return functor3<Vector, L, T>::call(clamp, vec, Vector<L, T>(minVec), Vector<L, T>(maxVec));
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> clamp(const Vector<L, T>& vec, const Vector<L, T>& minVec, const Vector<L, T>& maxVec) {
            return functor3<Vector, L, T>::call(clamp, vec, minVec, maxVec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> floor(const Vector<L, T>& vec) {
            return functor1<Vector, L, T, T>::call(floor, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> fract(const Vector<L, T>& vec) {
            return functor1<Vector, L, T, T>::call(fract, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> frexp(const Vector<L, T>& vec, const Vector<L, int>& exp) {
            Vector<L, T> v;
            for(uint32_t i=0; i<v.length(); i++) 
                v[i] = std::frexp(vec[i], &exp[i]);
            return v;
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, bool> isInf(const Vector<L, T>& vec) {
            return functor1<Vector, L, bool, T>::call(isInf, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, bool> isNan(const Vector<L, T>& vec) {
            return functor1<Vector, L, bool, T>::call(isNan, vec);
        }

        template<size_t L, typename T>
        inline constexpr Vector<L, T> ldexp(const Vector<L, T>& vec, const Vector<L, int>& exp) {
            Vector<L, T> v;
            for(uint32_t i=0; i<v.length(); i++) 
                v[i] = std::ldexp(vec[i], &exp[i]);
            return v;
        }

        template<size_t L, typename T>
		inline constexpr Vector<L, T> max(const Vector<L, T>& vec, T value) {
			return functor2<Vector, L, T>::call(max, vec, Vector<L, T>(value));
		}

        template<size_t L, typename T>
		inline constexpr Vector<L, T> max(T value, const Vector<L, T>& vec) {
			return functor2<Vector, L, T>::call(max, Vector<L, T>(value), vec);
		}

		template<size_t L, typename T>
		inline constexpr Vector<L, T> max(const Vector<L, T>& vec1, const Vector<L, T>& vec2) {
			return functor2<Vector, L, T>::call(max, vec1, vec2);
		}

        template<size_t L, typename T>
		inline constexpr Vector<L, T> min(const Vector<L, T>& vec, T value) {
			return functor2<Vector, L, T>::call(min, vec, Vector<L, T>(value));
		}

        template<size_t L, typename T>
		inline constexpr Vector<L, T> min(T value, const Vector<L, T>& vec) {
			return functor2<Vector, L, T>::call(min, Vector<L, T>(value), vec);
		}

		template<size_t L, typename T>
		inline constexpr Vector<L, T> min(const Vector<L, T>& vec1, const Vector<L, T>& vec2) {
			return functor2<Vector, L, T>::call(min, vec1, vec2);
		}

		template<size_t L, typename T, typename U>
		inline constexpr Vector<L, T> mix(const Vector<L, T>& vec1, const Vector<L, T>& vec2, U a) {
			return functor3<Vector, L, T>::call(mix, vec1, vec2, Vector<L, T>(a));
		}

        template<size_t L, typename T, typename U>
		inline constexpr Vector<L, T> mix(const Vector<L, T>& vec1, const Vector<L, T>& vec2, const Vector<L, U>& a) {
			return functor3<Vector, L, T>::call(mix, vec1, vec2, Vector<L, T>(a));
		}




    }
}