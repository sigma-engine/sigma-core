#ifndef VEC4_HPP
#define VEC4_HPP

#include <math/utils.hpp>

template<typename T>
struct vec2_t;
template<typename T>
struct vec3_t;

template <typename T>
struct vec4_t {
	T x,y,z,w;
	vec4_t(T x=0,T y=0,T z=0,T w=0)
		:x(x),y(y),z(z),w(w) {
	}

	vec4_t(const vec2_t<T> &v,T z=0,T w=0)
		:vec4_t<T>(v.x,v.y,z,w) {
	}

	vec4_t(const vec2_t<T> &v1,const vec2_t<T> &v2)
		:vec4_t<T>(v1,v2.x,v2.y) {
	}

	vec4_t(const vec3_t<T> &v,T w=0)
		:vec4_t<T>(v.x,v.y,v.z,w) {
	}

	bool operator ==(const vec4_t<T> &other) const {
		return almost_equal(x,other.x) && almost_equal(y,other.y) &&
				almost_equal(z,other.z) && almost_equal(w,other.w);
	}

	bool operator !=(const vec4_t<T> &other) const {
		return !almost_equal(x,other.x) || !almost_equal(y,other.y) ||
				!almost_equal(z,other.z) || !almost_equal(w,other.w);
	}

	inline bool operator<(const vec4_t<T> &other) const {
		return x<other.x&&y<other.y&&z<other.z&&w<other.w;
	}

	inline bool operator>(const vec4_t<T> &other) const {
		return x>other.x&&y>other.y&&z>other.z&&w>other.w;
	}

	inline bool operator<=(const vec4_t<T> &other) const {
		return x<=other.x&&y<=other.y&&z<=other.z&&w<=other.w;
	}

	inline bool operator>=(const vec4_t<T> &other) const {
		return x>=other.x&&y>=other.y&&z>=other.z&&w>=other.w;
	}

	inline T &operator[](int index) {
		return *((&x)+index);
	}

	inline T operator[](int index) const {
		return *((&x)+index);
	}

	inline vec4_t<T> operator+(const vec4_t<T> &other) const {
		return vec4_t<T>(x+other.x,y+other.y,z+other.z,w+other.w);
	}

	inline vec4_t<T> operator-(const vec4_t<T> &other) const {
		return vec4_t<T>(x-other.x,y-other.y,z-other.z,w-other.w);
	}

	inline vec4_t<T> operator-() const {
		return vec4_t<T>(-x,-y,-z,-w);
	}

	inline vec4_t<T> operator*(const T &s) const {
		return vec4_t<T>(x*s,y*s,z*s,w*s);
	}

	inline vec4_t<T> operator/(const T &s) const {
		return vec4_t<T>(x/s,y/s,z/s,w/s);
	}

	inline vec4_t<T> operator+=(const vec4_t<T> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline vec4_t<T> &operator-=(const vec4_t<T> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline vec4_t<T> &operator*=(const T &s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	inline vec4_t<T> &operator/=(const T &s) {
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	inline T length() const {
		return std::sqrt(x*x+y*y+z*z+w*w);
	}

	inline T square_length() const {
		return x*x+y*y+z*z+w*w;
	}

	inline vec4_t<T> get_normalized() {
		T mag = length();
		if(almost_equal(mag,(T)0))
			return vec4_t<T>();
		return vec4_t<T>(x,y,z,w)/mag;
	}

	inline T dot(const vec4_t<T> &other) const {
		return x*other.x+y*other.y+z*other.z+w*other.w;
	}

	inline vec4_t<T> &normalize() {
		*this = get_normalized();
		return *this;
	}
};

typedef vec4_t<int> int4;
typedef vec4_t<float> float4;
typedef vec4_t<double> double4;

template<typename T>
vec4_t<T> operator *(const T &s,const vec4_t<T> &v) {
	return v*s;
}

#endif //VEC4_HPP
