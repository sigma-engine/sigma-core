#ifndef MAT4X4_HPP
#define MAT4X4_HPP

#include <math/vec4.hpp>
#include <cmath>

template<typename T>
class quaternion_t;

template<typename T>
class mat4x4_t {
public:
	mat4x4_t() {
		data_[0] = { 1, 0, 0, 0 };
		data_[1] = { 0, 1, 0, 0 };
		data_[2] = { 0, 0, 1, 0 };
		data_[3] = { 0, 0, 0, 1 };
	}

	mat4x4_t(vec4_t<T> v1, vec4_t<T> v2, vec4_t<T> v3, vec4_t<T> v4) {
		data_[0] = v1;
		data_[1] = v2;
		data_[2] = v3;
		data_[3] = v4;
	}

	inline bool operator==(const mat4x4_t<T> &other) const {
		return data_[0]==other.data_[0] &&
				data_[1]==other.data_[1] &&
				data_[2]==other.data_[2] &&
				data_[3]==other.data_[3];
	}

	inline bool operator!=(const mat4x4_t<T> &other) const {
		return data_[0]!=other.data_[0] ||
				data_[1]!=other.data_[1] ||
				data_[2]!=other.data_[2] ||
				data_[3]!=other.data_[3];
	}

	inline bool operator<(const mat4x4_t<T> &other) const {
		return data_[0]<other.data_[0] &&
				data_[1]<other.data_[1] &&
				data_[2]<other.data_[2] &&
				data_[3]<other.data_[3];
	}

	inline bool operator>(const mat4x4_t<T> &other) const {
		return data_[0]>other.data_[0] &&
				data_[1]>other.data_[1] &&
				data_[2]>other.data_[2] &&
				data_[3]>other.data_[3];
	}

	inline bool operator<=(const mat4x4_t<T> &other) const {
		return data_[0]<=other.data_[0] &&
				data_[1]<=other.data_[1] &&
				data_[2]<=other.data_[2] &&
				data_[3]<=other.data_[3];
	}

	inline bool operator>=(const mat4x4_t<T> &other) const {
		return data_[0]>=other.data_[0] &&
				data_[1]>=other.data_[1] &&
				data_[2]>=other.data_[2] &&
				data_[3]>=other.data_[3];
	}

	inline vec4_t<T> &operator[](int row) {
		return data_[row];
	}

	inline vec4_t<T> operator[](int row) const {
		return data_[row];
	}

	inline mat4x4_t<T> operator+(const mat4x4_t<T> &other) const {
		return std::move(mat4x4_t<T>(data_[0]+other.data_[0],
								 data_[1]+other.data_[1],
								 data_[2]+other.data_[2],
								 data_[3]+other.data_[3]));
	}

	inline mat4x4_t<T> operator-(const mat4x4_t<T> &other) const {
		return std::move(mat4x4_t<T>(data_[0]-other.data_[0],
								 data_[1]-other.data_[1],
								 data_[2]-other.data_[2],
								 data_[3]-other.data_[3]));
	}

	inline mat4x4_t<T> operator-() const {
		return std::move(mat4x4_t<T>(-data_[0],
								 -data_[1],
								 -data_[2],
								 -data_[3]));
	}

	inline mat4x4_t<T> operator*(const mat4x4_t<T> &other) const {
		vec4_t<T> column0 = other.get_column(0);
		vec4_t<T> column1 = other.get_column(1);
		vec4_t<T> column2 = other.get_column(2);
		vec4_t<T> column3 = other.get_column(3);
		return std::move(mat4x4_t<T>({data_[0].dot(column0),data_[0].dot(column1),data_[0].dot(column2),data_[0].dot(column3)},
		{data_[1].dot(column0),data_[1].dot(column1),data_[1].dot(column2),data_[1].dot(column3)},
		{data_[2].dot(column0),data_[2].dot(column1),data_[2].dot(column2),data_[2].dot(column3)},
		{data_[3].dot(column0),data_[3].dot(column1),data_[3].dot(column2),data_[3].dot(column3)}));
	}

	inline vec4_t<T> operator*(const vec4_t<T> &other) const {
		return std::move(vec4_t<T>(data_[0].dot(other),data_[1].dot(other),data_[2].dot(other),data_[3].dot(other)));
	}

	inline mat4x4_t<T> operator*(const T &s) const {
		return std::move(mat4x4_t<T>(data_[0]*s,
								 data_[1]*s,
								 data_[2]*s,
								 data_[3]*s));
	}

	inline mat4x4_t<T> operator/(const T &s) const {
		return std::move(mat4x4_t<T>(data_[0]/s,
								 data_[1]/s,
								 data_[2]/s,
								 data_[3]/s));
	}

	inline mat4x4_t<T> &operator+=(const mat4x4_t<T> &other) {
		data_[0] += other.data_[0];
		data_[1] += other.data_[1];
		data_[2] += other.data_[2];
		data_[3] += other.data_[3];
		return *this;
	}

	inline mat4x4_t<T> &operator-=(const mat4x4_t<T> &other) {
		data_[0] -= other.data_[0];
		data_[1] -= other.data_[1];
		data_[2] -= other.data_[2];
		data_[3] -= other.data_[3];
		return *this;
	}

	inline mat4x4_t<T> &operator*=(const mat4x4_t<T> &other) {
		vec4_t<T> column0 = other.get_column(0);
		vec4_t<T> column1 = other.get_column(1);
		vec4_t<T> column2 = other.get_column(2);
		vec4_t<T> column3 = other.get_column(3);
		data_[0] = {data_[0].dot(column0),data_[0].dot(column1),data_[0].dot(column2),data_[0].dot(column3)};
		data_[1] = {data_[1].dot(column0),data_[1].dot(column1),data_[1].dot(column2),data_[1].dot(column3)};
		data_[2] = {data_[2].dot(column0),data_[2].dot(column1),data_[2].dot(column2),data_[2].dot(column3)};
		data_[3] = {data_[3].dot(column0),data_[3].dot(column1),data_[3].dot(column2),data_[3].dot(column3)};
		return *this;
	}

	inline mat4x4_t<T> &operator*=(const T &s) {
		data_[0] *= s;
		data_[1] *= s;
		data_[2] *= s;
		data_[3] *= s;
		return *this;
	}

	inline mat4x4_t<T> &operator/=(const T &s) {
		data_[0] /= s;
		data_[1] /= s;
		data_[2] /= s;
		data_[3] /= s;
		return *this;
	}

	inline vec4_t<T> get_column(int c) const {
		return std::move(vec4_t<T>(data_[0][c],data_[1][c],data_[2][c],data_[3][c]));
	}

	inline mat4x4_t<T> &set_column(int c, const vec4_t<T> &v) {
		data_[0][c] = v.x;
		data_[1][c] = v.y;
		data_[2][c] = v.z;
		data_[3][c] = v.w;
		return *this;
	}

	inline T det() const {
		return (data_[0].w*data_[1].z*data_[2].y*data_[3].x - data_[0].z*data_[1].w*data_[2].y*data_[3].x - data_[0].w*data_[1].y*data_[2].z*data_[3].x + data_[0].y*data_[1].w*data_[2].z*data_[3].x+
				data_[0].z*data_[1].y*data_[2].w*data_[3].x - data_[0].y*data_[1].z*data_[2].w*data_[3].x - data_[0].w*data_[1].z*data_[2].x*data_[3].y + data_[0].z*data_[1].w*data_[2].x*data_[3].y+
				data_[0].w*data_[1].x*data_[2].z*data_[3].y - data_[0].x*data_[1].w*data_[2].z*data_[3].y - data_[0].z*data_[1].x*data_[2].w*data_[3].y + data_[0].x*data_[1].z*data_[2].w*data_[3].y+
				data_[0].w*data_[1].y*data_[2].x*data_[3].z - data_[0].y*data_[1].w*data_[2].x*data_[3].z - data_[0].w*data_[1].x*data_[2].y*data_[3].z + data_[0].x*data_[1].w*data_[2].y*data_[3].z+
				data_[0].y*data_[1].x*data_[2].w*data_[3].z - data_[0].x*data_[1].y*data_[2].w*data_[3].z - data_[0].z*data_[1].y*data_[2].x*data_[3].w + data_[0].y*data_[1].z*data_[2].x*data_[3].w+
				data_[0].z*data_[1].x*data_[2].y*data_[3].w - data_[0].x*data_[1].z*data_[2].y*data_[3].w - data_[0].y*data_[1].x*data_[2].z*data_[3].w + data_[0].x*data_[1].y*data_[2].z*data_[3].w);
	}

	inline mat4x4_t<T> get_transpose() const {
		mat4x4_t<T> m;
		for(int i=0;i<4;++i) {
			for(int j=0;j<4;++j)
				m.data_[j][i] = data_[i][j];
		}
		return std::move(m);
	}

	inline mat4x4_t<T> get_inverse() const {
		T s = 1/det();
		return mat4x4_t<T>({(data_[1].z*data_[2].w*data_[3].y-data_[1].w*data_[2].z*data_[3].y+data_[1].w*data_[2].y*data_[3].z-data_[1].y*data_[2].w*data_[3].z-data_[1].z*data_[2].y*data_[3].w+data_[1].y*data_[2].z*data_[3].w)*s,
						(data_[0].w*data_[2].z*data_[3].y-data_[0].z*data_[2].w*data_[3].y-data_[0].w*data_[2].y*data_[3].z+data_[0].y*data_[2].w*data_[3].z+data_[0].z*data_[2].y*data_[3].w-data_[0].y*data_[2].z*data_[3].w)*s,
						(data_[0].z*data_[1].w*data_[3].y-data_[0].w*data_[1].z*data_[3].y+data_[0].w*data_[1].y*data_[3].z-data_[0].y*data_[1].w*data_[3].z-data_[0].z*data_[1].y*data_[3].w+data_[0].y*data_[1].z*data_[3].w)*s,
						(data_[0].w*data_[1].z*data_[2].y-data_[0].z*data_[1].w*data_[2].y-data_[0].w*data_[1].y*data_[2].z+data_[0].y*data_[1].w*data_[2].z+data_[0].z*data_[1].y*data_[2].w-data_[0].y*data_[1].z*data_[2].w)*s},
					   {(data_[1].w*data_[2].z*data_[3].x-data_[1].z*data_[2].w*data_[3].x-data_[1].w*data_[2].x*data_[3].z+data_[1].x*data_[2].w*data_[3].z+data_[1].z*data_[2].x*data_[3].w-data_[1].x*data_[2].z*data_[3].w)*s,
						(data_[0].z*data_[2].w*data_[3].x-data_[0].w*data_[2].z*data_[3].x+data_[0].w*data_[2].x*data_[3].z-data_[0].x*data_[2].w*data_[3].z-data_[0].z*data_[2].x*data_[3].w+data_[0].x*data_[2].z*data_[3].w)*s,
						(data_[0].w*data_[1].z*data_[3].x-data_[0].z*data_[1].w*data_[3].x-data_[0].w*data_[1].x*data_[3].z+data_[0].x*data_[1].w*data_[3].z+data_[0].z*data_[1].x*data_[3].w-data_[0].x*data_[1].z*data_[3].w)*s,
						(data_[0].z*data_[1].w*data_[2].x-data_[0].w*data_[1].z*data_[2].x+data_[0].w*data_[1].x*data_[2].z-data_[0].x*data_[1].w*data_[2].z-data_[0].z*data_[1].x*data_[2].w+data_[0].x*data_[1].z*data_[2].w)*s},
					   {(data_[1].y*data_[2].w*data_[3].x-data_[1].w*data_[2].y*data_[3].x+data_[1].w*data_[2].x*data_[3].y-data_[1].x*data_[2].w*data_[3].y-data_[1].y*data_[2].x*data_[3].w+data_[1].x*data_[2].y*data_[3].w)*s,
						(data_[0].w*data_[2].y*data_[3].x-data_[0].y*data_[2].w*data_[3].x-data_[0].w*data_[2].x*data_[3].y+data_[0].x*data_[2].w*data_[3].y+data_[0].y*data_[2].x*data_[3].w-data_[0].x*data_[2].y*data_[3].w)*s,
						(data_[0].y*data_[1].w*data_[3].x-data_[0].w*data_[1].y*data_[3].x+data_[0].w*data_[1].x*data_[3].y-data_[0].x*data_[1].w*data_[3].y-data_[0].y*data_[1].x*data_[3].w+data_[0].x*data_[1].y*data_[3].w)*s,
						(data_[0].w*data_[1].y*data_[2].x-data_[0].y*data_[1].w*data_[2].x-data_[0].w*data_[1].x*data_[2].y+data_[0].x*data_[1].w*data_[2].y+data_[0].y*data_[1].x*data_[2].w-data_[0].x*data_[1].y*data_[2].w)*s},
					   {(data_[1].z*data_[2].y*data_[3].x-data_[1].y*data_[2].z*data_[3].x-data_[1].z*data_[2].x*data_[3].y+data_[1].x*data_[2].z*data_[3].y+data_[1].y*data_[2].x*data_[3].z-data_[1].x*data_[2].y*data_[3].z)*s,
						(data_[0].y*data_[2].z*data_[3].x-data_[0].z*data_[2].y*data_[3].x+data_[0].z*data_[2].x*data_[3].y-data_[0].x*data_[2].z*data_[3].y-data_[0].y*data_[2].x*data_[3].z+data_[0].x*data_[2].y*data_[3].z)*s,
						(data_[0].z*data_[1].y*data_[3].x-data_[0].y*data_[1].z*data_[3].x-data_[0].z*data_[1].x*data_[3].y+data_[0].x*data_[1].z*data_[3].y+data_[0].y*data_[1].x*data_[3].z-data_[0].x*data_[1].y*data_[3].z)*s,
						(data_[0].y*data_[1].z*data_[2].x-data_[0].z*data_[1].y*data_[2].x+data_[0].z*data_[1].x*data_[2].y-data_[0].x*data_[1].z*data_[2].y-data_[0].y*data_[1].x*data_[2].z+data_[0].x*data_[1].y*data_[2].z)*s});
	}

	inline mat4x4_t<T> &transpose() {
		*this = std::move(get_transpose());
		return *this;
	}

	inline mat4x4_t<T> &inverse() {
		*this = std::move(get_inverse());
		return *this;
	}

	static mat4x4_t<T> orthographic(T left, T right, T bottom, T top, T near, T far) {

		return {{ 2/(right - left), 0			    , 0				 , -(right + left)/(right - left) },
				{ 0    		      , 2/(top - bottom), 0				 , -(top + bottom)/(top - bottom) },
				{ 0               , 0			    , -2/(far - near), -(far + near)/(far - near)     },
				{ 0			      , 0			    , 0				 , 1                              }
			   };
	}

	static mat4x4_t<T> perspective(T fovy, T aspect, T znear, T zfar) {
		T f = T(1)/std::tan(fovy/T(2));
		T clip = znear-zfar;

        return {{ f/aspect, 0, 				  0,                    0 },
                { 0,        f, 				  0,                    0 },
                { 0,        0, (zfar+znear)/clip, (2*zfar*znear)/clip },
                { 0,        0,                -1,                   0 }};
	};

	static mat4x4_t<T> scale(vec3_t<T> scale) {
        return {{scale.x,0,0,0},
                {0,scale.y,0,0},
                {0,0,scale.z},
                {0,0,0,1}};
    }

    static mat4x4_t<T> rotation(quaternion_t<T> rot) {
        return rot;
    }

    static mat4x4_t<T> translation(vec3_t<T> pos) {
        return {{1,0,0,pos.x},
                {0,1,0,pos.y},
                {0,0,1,pos.z},
                {0,0,0,1}};
    }
    
private:
	vec4_t<T> data_[4];
};
typedef mat4x4_t<int> int4x4;
typedef mat4x4_t<float> float4x4;
typedef mat4x4_t<double> double4x4;

#endif //MAT4X4_HPP

