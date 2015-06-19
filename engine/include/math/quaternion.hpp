#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <math/utils.hpp>
#include <math/vec3.hpp>
#include <math/mat3.hpp>
#include <math/mat4.hpp>

template<typename T>
struct vec2_t;
template<typename T>
struct vec3_t;
template<typename T>
struct vec4_t;

template <typename T>
struct quaternion_t {
	union {
		struct {
			vec3_t<T> vector;
			T real;
		};
		struct {
			T x,y,z,w;
		};
	};

    quaternion_t(T x=0,T y=0,T z=0,T w=1)
		:x(x),y(y),z(z),w(w) {
	}

    quaternion_t(const vec2_t<T> &v,T z=0,T w=1)
		:quaternion_t<T>(v.x,v.y,z,w) {
	}

	quaternion_t(const vec2_t<T> &v1,const vec2_t<T> &v2)
		:quaternion_t<T>(v1,v2.x,v2.y) {
	}

    quaternion_t(const vec3_t<T> &v,T w=1)
		:quaternion_t<T>(v.x,v.y,v.z,w) {
	}

	quaternion_t(const vec4_t<T> &v)
		:quaternion_t<T>(v.x,v.y,v.z,v.w) {
	}

	bool operator ==(const quaternion_t<T> &other) const {
		return almost_equal(x,other.x) && almost_equal(y,other.y) &&
				almost_equal(z,other.z) && almost_equal(w,other.w);
	}

	bool operator !=(const quaternion_t<T> &other) const {
		return !almost_equal(x,other.x) || !almost_equal(y,other.y) ||
				!almost_equal(z,other.z) || !almost_equal(w,other.w);
	}

	inline bool operator<(const quaternion_t<T> &other) const {
		return x<other.x&&y<other.y&&z<other.z&&w<other.w;
	}

	inline bool operator>(const quaternion_t<T> &other) const {
		return x>other.x&&y>other.y&&z>other.z&&w>other.w;
	}

	inline bool operator<=(const quaternion_t<T> &other) const {
		return x<=other.x&&y<=other.y&&z<=other.z&&w<=other.w;
	}

	inline bool operator>=(const quaternion_t<T> &other) const {
		return x>=other.x&&y>=other.y&&z>=other.z&&w>=other.w;
	}

	inline T &operator[](int index) {
		return *((&x)+index);
	}

	inline T operator[](int index) const {
		return *((&x)+index);
	}

	inline quaternion_t<T> operator+(const quaternion_t<T> &other) const {
		return quaternion_t<T>(x+other.x,y+other.y,z+other.z,w+other.w);
	}

	inline quaternion_t<T> operator-(const quaternion_t<T> &other) const {
		return quaternion_t<T>(x-other.x,y-other.y,z-other.z,w-other.w);
	}

	inline quaternion_t<T> operator-() const {
		return quaternion_t<T>(-x,-y,-z,-w);
	}

    inline quaternion_t<T> operator*(const quaternion_t<T> &other) const {
        return {real*other.vector + other.real*vector+ vector * other.vector,
                real*other.real - vector.dot(other.vector)};

	}

	inline quaternion_t<T> operator*(const T &s) const {
		return quaternion_t<T>(x*s,y*s,z*s,w*s);
	}

	inline quaternion_t<T> operator/(const T &s) const {
		return quaternion_t<T>(x/s,y/s,z/s,w/s);
	}

	inline quaternion_t<T> operator+=(const quaternion_t<T> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline quaternion_t<T> &operator-=(const quaternion_t<T> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline quaternion_t<T> &operator*=(const T &s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	inline quaternion_t<T> &operator/=(const T &s) {
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

    inline quaternion_t<T> get_normalized() const {
		T mag = length();
		if(almost_equal(mag,(T)0))
			return quaternion_t<T>();
		return quaternion_t<T>(x,y,z,w)/mag;
	}

    inline quaternion_t<T> get_conjugate() const {
		return quaternion_t<T>(-x,-y,-z,w);
	}

    inline quaternion_t<T> get_inverse() const {
		quaternion_t<T> q(-x,-y,-z,w);
		q /= q.square_length();
		return q;
	}

	inline T dot(const quaternion_t<T> &other) const {
		return x*other.x+y*other.y+z*other.z+w*other.w;
	}

	inline quaternion_t<T> &normalize() {
		*this = get_normalized();
		return *this;
	}

	inline quaternion_t<T> &conjugate() {
		*this = get_conjugate();
		return *this;
	}

	inline quaternion_t<T> &inverse() {
		*this = get_inverse();
		return *this;
	}

    operator mat3x3_t<T>() const {
        //https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Conversion_to_and_from_the_matrix_representation
        T ww = w*w;
        T xx = x*x;
        T yy = y*y;
        T zz = z*z;

        T xy = x*y;
        T wz = w*z;
        T xz = x*z;
        T wy = w*y;
        T yz = y*z;
        T wx = w*x;

        return {{ww+xx-yy-zz,2*(xy-wz)  ,2*(xz+wy)},
                {2*(xy+wz)  ,ww-xx+yy-zz,2*(yz-wx)},
                {2*(xz-wy)  ,2*(yz+wx)  ,ww-xx-yy+zz}};
    }

    operator mat4x4_t<T>() const {
        //https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#Conversion_to_and_from_the_matrix_representation
        T ww = w*w;
        T xx = x*x;
        T yy = y*y;
        T zz = z*z;

        T xy = x*y;
        T wz = w*z;
        T xz = x*z;
        T wy = w*y;
        T yz = y*z;
        T wx = w*x;

        return {{ww+xx-yy-zz,2*(xy-wz)  ,2*(xz+wy)  ,0},
                {2*(xy+wz)  ,ww-xx+yy-zz,2*(yz-wx)  ,0},
                {2*(xz-wy)  ,2*(yz+wx)  ,ww-xx-yy+zz,0},
                {0          ,0          ,0          ,1}};
    }

    vec3_t<T> rotate(vec3_t<T> v) const {
        return (*this * quaternion_t<T>(v,0) * this->get_inverse()).vector;
    }

    static quaternion_t<T> from_axis_angle(vec3_t<T> axis, T angle) {
        quaternion_t<T> q;
        q.vector = axis.normalize();
        q.vector *= std::sin(angle/T(2));
        q.real = std::cos(angle/T(2));
        return q;
    }
};
typedef quaternion_t<int> quaternioni;
typedef quaternion_t<float> quaternionf;
typedef quaternion_t<double> quaterniond;

template<typename T>
quaternion_t<T> operator *(const T &s,const quaternion_t<T> &v) {
	return v*s;
}

#endif //QUATERNION_HPP
