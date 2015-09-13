#ifndef VEC3_HPP
#define VEC3_HPP

#include <sigmafive/math/utils.hpp>

template<typename T>
struct vec2_t;
template<typename T>
struct vec4_t;

template<typename T>
struct vec3_t {

    static const vec3_t<T> RIGHT;
    static const vec3_t<T> UP;
    static const vec3_t<T> OUTWARD;
    T x, y, z;

    vec3_t(T x = 0, T y = 0, T z = 0)
            : x(x), y(y), z(z) {
    }

    vec3_t(const vec2_t<T> &v, T z = 0)
            : vec3_t<T>(v.x, v.y, z) {
    }

    vec3_t(const vec4_t<T> &v)
            : vec3_t<T>(v.x, v.y, v.z) {
    }

    bool operator==(const vec3_t<T> &other) const {
        return almost_equal(x, other.x) && almost_equal(y, other.y) &&
               almost_equal(z, other.z);
    }

    bool operator!=(const vec3_t<T> &other) const {
        return !almost_equal(x, other.x) || !almost_equal(y, other.y) ||
               !almost_equal(z, other.z);
    }

    inline bool operator<(const vec3_t<T> &other) const {
        return x < other.x && y < other.y && z < other.z;
    }

    inline bool operator>(const vec3_t<T> &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    inline bool operator<=(const vec3_t<T> &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    inline bool operator>=(const vec3_t<T> &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    inline T &operator[](int index) {
        return *((&x) + index);
    }

    inline T operator[](int index) const {
        return *((&x) + index);
    }

    inline vec3_t<T> operator+(const vec3_t<T> &other) const {
        return vec3_t<T>(x + other.x, y + other.y, z + other.z);
    }

    inline vec3_t<T> operator-(const vec3_t<T> &other) const {
        return vec3_t<T>(x - other.x, y - other.y, z - other.z);
    }

    inline vec3_t<T> operator-() const {
        return vec3_t<T>(-x, -y, -z);
    }

    inline vec3_t<T> operator*(const T &s) const {
        return vec3_t<T>(x * s, y * s, z * s);
    }

    inline vec3_t<T> operator*(const vec3_t<T> &other) const {
        return vec3_t<T>((y * other.z) - (z * other.y),
                         (z * other.x) - (x * other.z),
                         (x * other.y) - (y * other.x));
    }

    inline vec3_t<T> operator/(const T &s) const {
        return vec3_t<T>(x / s, y / s, z / s);
    }

    inline vec3_t<T> operator+=(const vec3_t<T> &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline vec3_t<T> &operator-=(const vec3_t<T> &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline vec3_t<T> &operator*=(const T &s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    inline vec3_t<T> &operator/=(const T &s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    inline T length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline T square_length() const {
        return x * x + y * y + z * z;
    }

    inline vec3_t<T> get_normalized() {
        T mag = length();
        if (almost_equal(mag, (T) 0))
            return vec3_t<T>();
        return vec3_t<T>(x, y, z) / mag;
    }

    inline T dot(const vec3_t<T> &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    inline vec3_t<T> &normalize() {
        *this = get_normalized();
        return *this;
    }
};

typedef vec3_t<int> int3;
typedef vec3_t<float> float3;
typedef vec3_t<double> double3;

template<typename T>
const vec3_t<T> vec3_t<T>::RIGHT = {1, 0, 0};
template<typename T>
const vec3_t<T> vec3_t<T>::UP = {0, 1, 0};
template<typename T>
const vec3_t<T> vec3_t<T>::OUTWARD = {0, 0, 1};

template<typename T>
vec3_t<T> operator*(const T &s, const vec3_t<T> &v) {
    return v * s;
}

#endif //VEC3_HPP
