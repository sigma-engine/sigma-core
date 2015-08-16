#ifndef MAT3_HPP
#define MAT3_HPP

#include <sigmafive/math/vec3.hpp>

template<typename T>
class mat3x3_t {
public:
	mat3x3_t() {
		data_[0] = { 1, 0, 0 };
		data_[1] = { 0, 1, 0 };
		data_[2] = { 0, 0, 1 };
	}

	mat3x3_t(vec3_t<T> v1,vec3_t<T> v2,vec3_t<T> v3) {
		data_[0] = v1;
		data_[1] = v2;
		data_[2] = v3;
	}

	inline bool operator==(const mat3x3_t<T> &other) const {
		return data_[0]==other.data_[0] &&
				data_[1]==other.data_[1] &&
				data_[2]==other.data_[2];
	}

	inline bool operator!=(const mat3x3_t<T> &other) const {
		return data_[0]!=other.data_[0] ||
				data_[1]!=other.data_[1] ||
				data_[2]!=other.data_[2];
	}

	inline bool operator<(const mat3x3_t<T> &other) const {
		return data_[0]<other.data_[0] &&
				data_[1]<other.data_[1] &&
				data_[2]<other.data_[2];
	}

	inline bool operator>(const mat3x3_t<T> &other) const {
		return data_[0]>other.data_[0] &&
				data_[1]>other.data_[1] &&
				data_[2]>other.data_[2];
	}

	inline bool operator<=(const mat3x3_t<T> &other) const {
		return data_[0]<=other.data_[0] &&
				data_[1]<=other.data_[1] &&
				data_[2]<=other.data_[2];
	}

	inline bool operator>=(const mat3x3_t<T> &other) const {
		return data_[0]>=other.data_[0] &&
				data_[1]>=other.data_[1] &&
				data_[2]>=other.data_[2];
	}

	inline vec3_t<T> &operator[](int row) {
		return data_[row];
	}

	inline vec3_t<T> operator[](int row) const {
		return data_[row];
	}

	inline mat3x3_t<T> operator+(const mat3x3_t<T> &other) const {
		return std::move(mat3x3_t<T>(data_[0]+other.data_[0],
								 data_[1]+other.data_[1],
								 data_[2]+other.data_[2]));
	}

	inline mat3x3_t<T> operator-(const mat3x3_t<T> &other) const {
		return std::move(mat3x3_t<T>(data_[0]-other.data_[0],
								 data_[1]-other.data_[1],
								 data_[2]-other.data_[2]));
	}

	inline mat3x3_t<T> operator-() const {
		return std::move(mat3x3_t<T>(-data_[0],
								 -data_[1],
								 -data_[2]));
	}

	inline mat3x3_t<T> operator*(const mat3x3_t<T> &other) const {
		vec3_t<T> column0 = other.get_column(0);
		vec3_t<T> column1 = other.get_column(1);
		vec3_t<T> column2 = other.get_column(2);
		return std::move(mat3x3_t<T>({data_[0].dot(column0),data_[0].dot(column1),data_[0].dot(column2)},
								 {data_[1].dot(column0),data_[1].dot(column1),data_[1].dot(column2)},
								 {data_[2].dot(column0),data_[2].dot(column1),data_[2].dot(column2)}));
	}

	inline vec3_t<T> operator*(const vec3_t<T> &other) const {
		return std::move(vec3_t<T>(data_[0].dot(other),data_[1].dot(other),data_[2].dot(other)));
	}

	inline mat3x3_t<T> operator*(const T &s) const {
		return std::move(mat3x3_t<T>(data_[0]*s,
								 data_[1]*s,
								 data_[2]*s));
	}

	inline mat3x3_t<T> operator/(const T &s) const {
		return std::move(mat3x3_t<T>(data_[0]/s,
								 data_[1]/s,
								 data_[2]/s));
	}

	inline mat3x3_t<T> &operator+=(const mat3x3_t<T> &other) {
		data_[0] += other.data_[0];
		data_[1] += other.data_[1];
		data_[2] += other.data_[2];
		return *this;
	}

	inline mat3x3_t<T> &operator-=(const mat3x3_t<T> &other) {
		data_[0] -= other.data_[0];
		data_[1] -= other.data_[1];
		data_[2] -= other.data_[2];
		return *this;
	}

	inline mat3x3_t<T> &operator*=(const mat3x3_t<T> &other) {
		vec3_t<T> column0 = other.get_column(0);
		vec3_t<T> column1 = other.get_column(1);
		vec3_t<T> column2 = other.get_column(2);
		data_[0] = {data_[0].dot(column0),data_[0].dot(column1),data_[0].dot(column2)};
		data_[1] = {data_[1].dot(column0),data_[1].dot(column1),data_[1].dot(column2)};
		data_[2] = {data_[2].dot(column0),data_[2].dot(column1),data_[2].dot(column2)};
		return *this;
	}

	inline mat3x3_t<T> &operator*=(const T &s) {
		data_[0] *= s;
		data_[1] *= s;
		data_[2] *= s;
		return *this;
	}

	inline mat3x3_t<T> &operator/=(const T &s) {
		data_[0] /= s;
		data_[1] /= s;
		data_[2] /= s;
		return *this;
	}

	inline vec3_t<T> get_column(int c) const {
		return std::move(vec3_t<T>(data_[0][c],data_[1][c],data_[2][c]));
	}

	inline vec3_t<T> &set_column(int c, const vec3_t<T> &v) {
		data_[0][c] = v.x;
		data_[1][c] = v.y;
		data_[2][c] = v.z;
		return *this;
	}

	inline T det() const {
		return data_[0].x*(data_[1].y*data_[2].z-data_[2].y*data_[1].z)-
			   data_[0].y*(data_[1].x*data_[2].z-data_[2].x*data_[1].z)+
			   data_[0].z*(data_[1].x*data_[2].y-data_[2].x*data_[1].y);
	}

	inline mat3x3_t<T> get_transpose() const {
		mat3x3_t<T> m;
		for(int i=0;i<3;++i) {
			for(int j=0;j<3;++j)
				m.data_[j][i] = data_[i][j];
		}
		return std::move(m);
	}

	inline mat3x3_t<T> get_inverse() const {
		T s = 1/det();
		return mat3x3_t<T>({(( data_[1][1]*data_[2][2]-data_[2][1]*data_[1][2]))*s,
						(-(data_[0][1]*data_[2][2]-data_[2][1]*data_[0][2]))*s,
						(( data_[0][1]*data_[1][2]-data_[1][1]*data_[0][2]))*s},
					   {(-(data_[1][0]*data_[2][2]-data_[2][0]*data_[1][2]))*s,
						(( data_[0][0]*data_[2][2]-data_[2][0]*data_[0][2]))*s,
						(-(data_[0][0]*data_[1][2]-data_[1][0]*data_[0][2]))*s},
					   {(( data_[1][0]*data_[2][1]-data_[2][0]*data_[1][1]))*s,
						(-(data_[0][0]*data_[2][1]-data_[2][0]*data_[0][1]))*s,
						(( data_[0][0]*data_[1][1]-data_[1][0]*data_[0][1]))*s});
	}

	inline mat3x3_t<T> &transpose() {
		*this = std::move(get_transpose());
		return *this;
	}

	inline mat3x3_t<T> &inverse() {
		*this = std::move(get_inverse());
		return *this;
	}
private:
	vec3_t<T> data_[3];
};

typedef mat3x3_t<int> int3x3;
typedef mat3x3_t<float> float3x3;
typedef mat3x3_t<double> double3x3;

#endif //MAT3_HPP
