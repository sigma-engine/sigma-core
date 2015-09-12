#ifndef VEC2_HPP
#define VEC2_HPP

#include <sigmafive/math/utils.hpp>

template<typename T>
struct vec3_t;
template<typename T>
struct vec4_t;

template <typename T>
struct vec2_t {
	T x,y;

	vec2_t(T x=0,T y=0)
		:x(x),y(y) {
	}

	vec2_t(const vec3_t<T> &v)
		:vec2_t<T>(v.x,v.y) {
	}

	vec2_t(const vec4_t<T> &v)
		:vec2_t<T>(v.x,v.y) {
	}

	bool operator ==(const vec2_t<T> &other) const {
		return almost_equal(x,other.x) && almost_equal(y,other.y);
	}

	bool operator !=(const vec2_t<T> &other) const {
		return !almost_equal(x,other.x) || !almost_equal(y,other.y);
	}

	inline bool operator<(const vec2_t<T> &other) const {
		return x<other.x&&y<other.y;
	}

	inline bool operator>(const vec2_t<T> &other) const {
		return x>other.x&&y>other.y;
	}

	inline bool operator<=(const vec2_t<T> &other) const {
		return x<=other.x&&y<=other.y;
	}

	inline bool operator>=(const vec2_t<T> &other) const {
		return x>=other.x&&y>=other.y;
	}

	inline T &operator[](int index) {
		return *((&x)+index);
	}

	inline T operator[](int index) const {
		return *((&x)+index);
	}

	inline vec2_t<T> operator+(const vec2_t<T> &other) const {
		return vec2_t<T>(x+other.x,y+other.y);
	}

	inline vec2_t<T> operator-(const vec2_t<T> &other) const {
		return vec2_t<T>(x-other.x,y-other.y);
	}

	inline vec2_t<T> operator-() const {
		return vec2_t<T>(-x,-y);
	}

	inline vec2_t<T> operator*(const T &s) const {
		return vec2_t<T>(x*s,y*s);
	}

	inline vec2_t<T> operator/(const T &s) const {
		return vec2_t<T>(x/s,y/s);
	}

	inline vec2_t<T> operator+=(const vec2_t<T> &other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	inline vec2_t<T> &operator-=(const vec2_t<T> &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline vec2_t<T> &operator*=(const T &s) {
		x *= s;
		y *= s;
		return *this;
	}

	inline vec2_t<T> &operator/=(const T &s) {
		x /= s;
		y /= s;
		return *this;
	}

	inline T length() const {
		return std::sqrt(x*x+y*y);
	}

	inline T square_length() const {
		return x*x+y*y;
	}

	inline vec2_t<T> get_normalized() {
		T mag = length();
		if(almost_equal(mag,(T)0))
			return vec2_t<T>();
		return vec2_t<T>(x,y)/mag;
	}

	inline T dot(const vec2_t<T> &other) const {
		return x*other.x+y*other.y;
	}

	inline vec2_t<T> &normalize() {
		*this = get_normalized();
		return *this;
	}
};

typedef vec2_t<int> int2;
typedef vec2_t<float> float2;
typedef vec2_t<double> double2;

template<typename T>
vec2_t<T> operator *(const T &s,const vec2_t<T> &v) {
	return v*s;
}

#endif //VEC2_HPP
