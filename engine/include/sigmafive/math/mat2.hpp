#ifndef MAT2X2_HPP
#define MAT2X2_HPP

#include <sigmafive/math/vec2.hpp>

template<typename T>
class mat2x2_t {
public:
	mat2x2_t() {
		data_[0] = { 1, 0 };
		data_[1] = { 0, 1 };
	}

	mat2x2_t(vec2_t<T> v1,vec2_t<T> v2) {
		data_[0] = v1;
		data_[1] = v2;
	}

	inline bool operator==(const mat2x2_t<T> &other) const {
		return data_[0]==other.data_[0] &&
				data_[1]==other.data_[1];
	}

	inline bool operator!=(const mat2x2_t<T> &other) const {
		return data_[0]!=other.data_[0] ||
				data_[1]!=other.data_[1];
	}

	inline bool operator<(const mat2x2_t<T> &other) const {
		return data_[0]<other.data_[0] &&
				data_[1]<other.data_[1];
	}

	inline bool operator>(const mat2x2_t<T> &other) const {
		return data_[0]>other.data_[0] &&
				data_[1]>other.data_[1];
	}

	inline bool operator<=(const mat2x2_t<T> &other) const {
		return data_[0]<=other.data_[0] &&
				data_[1]<=other.data_[1];
	}

	inline bool operator>=(const mat2x2_t<T> &other) const {
		return data_[0]>=other.data_[0] &&
				data_[1]>=other.data_[1];
	}

	inline vec2_t<T> &operator[](int row) {
		return data_[row];
	}

	inline vec2_t<T> operator[](int row) const {
		return data_[row];
	}

	inline mat2x2_t<T> operator+(const mat2x2_t<T> &other) const {
		return std::move(mat2x2_t<T>(data_[0]+other.data_[0],
								 data_[1]+other.data_[1]));
	}

	inline mat2x2_t<T> operator-(const mat2x2_t<T> &other) const {
		return std::move(mat2x2_t<T>(data_[0]-other.data_[0],
								 data_[1]-other.data_[1]));
	}

	inline mat2x2_t<T> operator-() const {
		return std::move(mat2x2_t<T>(-data_[0],
								 -data_[1]));
	}

	inline mat2x2_t<T> operator*(const mat2x2_t<T> &other) const {
		vec2_t<T> column0 = other.get_column(0);
		vec2_t<T> column1 = other.get_column(1);
		return std::move(mat2x2_t<T>({data_[0].dot(column0),data_[0].dot(column1)},
								 {data_[1].dot(column0),data_[1].dot(column1)}));
	}

	inline vec2_t<T> operator*(const vec2_t<T> &other) const {
		return std::move(vec2_t<T>(data_[0].dot(other),data_[1].dot(other)));
	}

	inline mat2x2_t<T> operator*(const T &s) const {
		return std::move(mat2x2_t<T>(data_[0]*s,
								 data_[1]*s));
	}

	inline mat2x2_t<T> operator/(const T &s) const {
		return std::move(mat2x2_t<T>(data_[0]/s,
								 data_[1]/s));
	}

	inline mat2x2_t<T> &operator+=(const mat2x2_t<T> &other) {
		data_[0] += other.data_[0];
		data_[1] += other.data_[1];
		return *this;
	}

	inline mat2x2_t<T> &operator-=(const mat2x2_t<T> &other) {
		data_[0] -= other.data_[0];
		data_[1] -= other.data_[1];
		return *this;
	}

	inline mat2x2_t<T> &operator*=(const mat2x2_t<T> &other) {
		vec2_t<T> column0 = other.get_column(0);
		vec2_t<T> column1 = other.get_column(1);
		data_[0] = {data_[0].dot(column0),data_[0].dot(column1)};
		data_[1] = {data_[1].dot(column0),data_[1].dot(column1)};
		return *this;
	}

	inline mat2x2_t<T> &operator*=(const T &s) {
		data_[0] *= s;
		data_[1] *= s;
		return *this;
	}

	inline mat2x2_t<T> &operator/=(const T &s) {
		data_[0] /= s;
		data_[1] /= s;		
		return *this;
	}

	inline vec2_t<T> get_column(int c) const {
		return vec2_t<T>(data_[0][c],data_[1][c]);
	}

	inline vec2_t<T> &set_column(int c, const vec2_t<T> &v) {
		data_[0][c] = v.x;
		data_[1][c] = v.y;
		return *this;
	}

	inline T det() const {
		return data_[0].x*data_[1].y-data_[1].x*data_[0].y;
	}

	inline mat2x2_t<T> get_transpose() const {
		mat2x2_t<T> m;
		for(int i=0;i<2;++i) {
			for(int j=0;j<2;++j)
				m.data_[j][i] = data_[i][j];
		}
		return std::move(m);
	}

	inline mat2x2_t<T> get_inverse() const {
		T s = det();
		return mat2x2_t<T>({data_[1].y/s,-data_[0].y/s},
					   {-data_[1].x/s,data_[0].x/s});
	}

	inline mat2x2_t<T> &transpose() {
		*this = std::move(get_transpose());
		return *this;
	}

	inline mat2x2_t<T> &inverse() {
		*this = std::move(get_inverse());
		return *this;
	}
private:
	vec2_t<T> data_[2];
};

typedef mat2x2_t<int> int2x2;
typedef mat2x2_t<float> float2x2;
typedef mat2x2_t<double> double2x2;

#endif //MAT2X2_HPP
